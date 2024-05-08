#!/usr/bin/env python3
#-*- coding:utf-8 -*-
# This Source Code is subject to the terms of
# TURBOTOUCH PREDICTOR version 1.0 licence
# available in LICENCE.txt
# Author: Sebastien Poulmane

import argparse, os, re, subprocess, sys, time

# ---> Internal parameters
OPTIMAL_TEMP_FILENAME = "optim.csv" # Temporary file that contains optimal parameters of current Matlab processing
OPTIMALX_FILENAME = "optim_x.csv" # File containing optimal alpha, beta and theta for x axis
OPTIMALY_FILENAME = "optim_y.csv" # File containing optimal alpha, beta and theta for y axis
OPTPARAM_FILENAME = "optparam.cfg" # CFG file containing parameters for predictor
TRAINING_FILENAME = "train.csv" # File containing all inputs (time, x, y, events) for training
GET_TRAIN_INDEX = [1,3]

# ---- Process args
parser = argparse.ArgumentParser(prog="Optimized Predictor")
parser.add_argument('input',
	help="Directory containing CSV files of raw inputs (timestamp, x, y, ...)")
parser.add_argument('fs',
	help="Base frequency of the device")
parser.add_argument('l0',
	help="Latency")
parser.add_argument('-w1',
	default="1",
	help="Weight 1")
parser.add_argument('-w2',
	default="1",
	help="Weight 2")
parser.add_argument('-w3',
	default="1",
	help="Weight 3")
parser.add_argument('-o', '--output',
	help="Output directory. If not provided, create the output into the current \
working directory.")
parser.add_argument('-f', '--format',
	default="comma",
	help="Data format of the gesture files \
[comma (default), playground]")
parser.add_argument('-s', '--symetric', action="store_true", help="If enable, Y parameters will use X parameters. Make tuning much faster. But may it less precise.")
parser.add_argument('-m', '--merged', action="store_true", help="Enable this if the input contain an unique file already merged. So the program will fix timestamp to make them chronologically correct.")
parser.add_argument('-v', '--verbose', action="store_true")
parser.add_argument('-t', '--testpart', action="store_true",
	help="If enable, some gesture files into the directory are kept from the \
tuning process in order to make predict test. \
Each file has a number at the end of its name. \
Modify GET_TRAIN_INDEX to select which files are used for train. The \
other will be used for test.")
args = parser.parse_args()

if args.verbose:
	print("Start job for %s..." % args.input)

if args.output:
	output_path = args.output
else:
	if os.path.isdir(args.input):
		output_path = args.input
	else:
		output_path = os.path.dirname(args.input)
train_path = os.path.join(output_path, TRAINING_FILENAME)
optim_temp_path = os.path.join(output_path, OPTIMAL_TEMP_FILENAME)
optimx_path = os.path.join(output_path, OPTIMALX_FILENAME)
optimy_path = os.path.join(output_path, OPTIMALY_FILENAME)
optparam_path = os.path.join(output_path, OPTPARAM_FILENAME)

# ---- Get files and gestures
gestures = []
if os.path.isdir(args.input):
	dirpath = args.input
	inputlist = os.listdir(args.input)
else:
	dirpath = "."
	inputlist = [args.input]
trainlist = []
testlist = []
for filename in inputlist:
	if args.testpart:
		m = re.match(r'([A-Za-z \(\)]+)_([0-9]+)\.csv', filename)
		if m is not None:
			trial_id = int(m.group(2))
			if trial_id in GET_TRAIN_INDEX:
				trainlist.append(filename)
			else:
				testlist.append(filename)
	else:
		m = re.match(r'\w+\.csv', filename)
		if m is not None:
			trainlist.append(filename)
if len(trainlist) == 0:
	print("Error: Directory {} is empty or \
does not contain CSV file.".format(dirpath))
	exit(-1)

if args.verbose:
	print("Get %s files for training..." % len(trainlist))

# ---- Read strokes
	## Looking for Optimized Parameters in directory
	## Generate it automaticaly if not exist
if not os.path.isfile(optparam_path):
	if not os.path.isfile(train_path):
		# Read strokes and merge them
		if not os.path.isfile(train_path):
			print("Creating training file...")
			rows = []
			if args.format == "playground":
				prev_event = "OutOfRange"
			for trainfile in trainlist:
				begin_gesture = True
				with open(os.path.join(dirpath, trainfile)) as inputfile:
					inputfile.readline() # Skip header
					for line in inputfile.readlines():
						if args.format == "comma":
							values = line.split(',')
							raw_pos = (float(values[1]), float(values[2]))
							raw_time = float(values[0])/1e9 # NOTE Timestamp converted to seconds from µs
							event = values[3].strip()
						else: # playground
							values = line.split(';')
							raw_pos = (float(values[2]), float(values[3]))
							raw_time = float(values[0])/1e9 # NOTE Timestamp converted to seconds from ns
							cur_event = values[4].strip()
							if prev_event != "Interacting" and cur_event != "Interacting": # Ignore out of range points
								continue
							elif prev_event != "Interacting" and cur_event == "Interacting":
								event = "DOWN"
							elif prev_event == "Interacting" and cur_event != "Interacting":
								event = "UP"
							else:
								event = "MOVE"
							prev_event = cur_event
						if begin_gesture or args.merged and event == "DOWN":
							if len(rows):
								shift_time = raw_time - rows[-1][0] - 1.0/float(args.fs)
							else:
								shift_time = raw_time
							begin_gesture = False
						rows.append([raw_time-shift_time, raw_pos[0], raw_pos[1], event])
			# Write to file
			with open(train_path, 'w') as trainfile:
				for row in rows:
					trainfile.write(";".join(map(str, row)) + "\n")
		else:
			print("Training file already exists. Skipped.")

		# ---- Optimizing
		# NOTE Below, this is the code to call pure Python optimization. For now it's matlab :/
		#optim_content = subprocess.run(["./single_axes_opt.py",
		#	os.path.join(args.input, TRAINING_FILENAME), "-s", ",",
		#	"-t", "0", "-x", "1", "-y", "2"],
		#	stdout=subprocess.PIPE)

		if args.verbose:
			print("Start optimizations...")
			time_start = time.perf_counter()
			# ---- Axis X
		if not os.path.isfile(optimx_path):
			print("Optimizing axis X...")
			optim_error = subprocess.call(["matlab -nosplash -nodisplay \
		-r \"process_A(\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\');\
			quit;\"" % (output_path, args.fs, args.l0, 'x', args.w1, args.w2, args.w3)], shell=True)
			if optim_error:
				print("Error: Matlab failed to compute optim_x. Process aboarded.")
				exit(-1)
			else:
				os.rename(optim_temp_path, optimx_path)
				print("Otpimization for axis X done.")
		else:
			print("Optimization for axis X already exists. Skipped.")

			# ---- Axis Y
		if args.symetric:
			print("Copy X parameters to Y. Axis Y optimization skipped.")
		elif not os.path.isfile(optimy_path):
			print("Optimizing axis Y...")
			optim_error = subprocess.call(["matlab -nosplash -nodisplay \
		-r \"process_A(\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\');\
			quit;\"" % (output_path, args.fs, args.l0, 'y', args.w1, args.w2, args.w3)], shell=True)
			if optim_error:
				print("Error: Matlab failed to compute optim_y. Process aboarded.")
				exit(-1)
			else:
				os.rename(optim_temp_path, optimy_path)
				print("Otpimization for axis Y done.")
		else:
			print("Optimization for axis Y already exists. Skipped.")

	else:
		print("Otpimal CSV already exists. Skipped.")

	# ---- Create CFG file
	with open(optparam_path, "w") as paramfile:
		buffer = "# " + args.input + " - fq:" + args.fs + " - lag:" + args.l0 + "\n\n"
		buffer += "predictor: turbotouch:\n"
		buffer += "output-time: no\n"
		buffer += "turbotouch-V0: 2000\n" # Default value.
		buffer += "turbotouch-ts: %s\n" % str(1.0/float(args.fs))
		buffer += "turbotouch-alpha_gain: %s\n" % str(.04**.1)
		buffer += "turbotouch-gamma1-x: 1\nturbotouch-gamma1-y: 1\n"
		buffer += "turbotouch-weights: %s %s %s\n" % (args.w1, args.w2, args.w3)
		with open(os.path.join(output_path, "optim_x.csv"), "r") as optfile:
			buffer += "turbotouch-alpha-x: " + optfile.readline().strip() + "\n"
			buffer += "turbotouch-lambda-x: " + optfile.readline().strip() + "\n"
			buffer += "turbotouch-gamma2-x: " + optfile.readline().strip() + "\n"
			buffer += "turbotouch-gamma3-x: " + optfile.readline().strip() + "\n"
			buffer += "turbotouch-gamma4-x: " + optfile.readline().strip() + "\n"
		if args.symetric:
			with open(os.path.join(output_path, "optim_x.csv"), "r") as optfile:
				buffer += "turbotouch-alpha-y: " + optfile.readline().strip() + "\n"
				buffer += "turbotouch-lambda-y: " + optfile.readline().strip() + "\n"
				buffer += "turbotouch-gamma2-y: " + optfile.readline().strip() + "\n"
				buffer += "turbotouch-gamma3-y: " + optfile.readline().strip() + "\n"
				buffer += "turbotouch-gamma4-y: " + optfile.readline().strip() + "\n"
		else:
			with open(os.path.join(output_path, "optim_y.csv"), "r") as optfile:
				buffer += "turbotouch-alpha-y: " + optfile.readline().strip() + "\n"
				buffer += "turbotouch-lambda-y: " + optfile.readline().strip() + "\n"
				buffer += "turbotouch-gamma2-y: " + optfile.readline().strip() + "\n"
				buffer += "turbotouch-gamma3-y: " + optfile.readline().strip() + "\n"
				buffer += "turbotouch-gamma4-y: " + optfile.readline().strip() + "\n"
		paramfile.write(buffer)
		# TODO Get this values from a separate file or in the parameters

	if args.verbose:
		print("Perform predictor optimization in {:.3f} seconds.".format(
			time.perf_counter() - time_start
		))

# ---- Make predictions
if args.testpart:
	if args.verbose:
		time_start = time.perf_counter()

	print("Predicting...")
	for inputfile in os.listdir(args.input):
		if not inputfile.startswith("predict-"):
			m = re.match(r'([A-Za-z_ \(\)]+)_([0-9]+)\.csv', inputfile)
			if m is not None:
				sys.stdout.write("\r%s\rProcessing %s..." % (" "*80, inputfile))
				sys.stdout.flush()
				input_path = os.path.join(args.input, inputfile)
				output_file = os.path.join(output_path, "predict-"+inputfile)
				predict_return = subprocess.call(["./predict-tester",
					optparam_path, input_path, output_file])
				if predict_return:
					print("Error occured. Can't done prediction.")
	sys.stdout.write("\r%s\rDone.\n" % (" "*80))
	sys.stdout.flush()

	if args.verbose:
		print("Job finish for %s!" % args.input)
		print("Perform all predictions in {:.3f} seconds.".format(
			time.perf_counter() - time_start
		))
