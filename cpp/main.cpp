/* -*- mode: c++ -*-
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Gery Casiez
 *
 */

#include <lag/predictor/Predictor.h>
#include <lag/utils/TimeStamp.h>
#include <lag/input/processing/InputEventProcessor.h>
#include <fstream>

using namespace lag;
using namespace std;

Predictor *pred;
bool useInputEventProcessor;
InputEventProcessor *processor;

void reset(Predictor *pred) {
	pred->reset();
}

InputEvent predict(std::string timestamp, double x, double y, std::string state) {
	InputEvent predictedEvent;
	InputEvent::State s = InputEvent::Interacting ;

	if ((state == "DOWN") || (state == "UP")) {
		s = InputEvent::OutOfRange;
		if (useInputEventProcessor) processor->reset();
	}

	TimeStamp::inttime t = TimeStamp::createAsIntFrom(timestamp);
	InputEvent e = InputEvent(0,
				   InputEvent::DefaultChannel,
				   InputEvent::Absolute,		  
				   s,
			       t, x, y);

	predictedEvent = pred->update(e, 0);

	if (useInputEventProcessor)
    	predictedEvent = processor->process(&predictedEvent, &e) ;
    return predictedEvent;
}

int main(int argc, char* argv[]){
	useInputEventProcessor = false;
	string cfgfile;

	if (argc == 2)
		cfgfile = argv[1];
	else
		cfgfile = "config/turbotouch64.cfg";

	ConfigDict cfg = ConfigDict (cfgfile);
	pred = Predictor::create ("turbotouch:", &cfg);

	useInputEventProcessor = cfg.get<std::string>("processor-enable") == "true";

	if (useInputEventProcessor) {
		processor = InputEventProcessor::create(cfg.get<std::string>("processor"), &cfg) ;
		std::cout << "Using processor " << processor->getURI().asString() << std::endl;
	}

	ifstream file ("../strokes.csv");
	ofstream fileO ("./strokesPredicted.csv");
	string value;
	getline (file, value, '\n' );
	fileO << value.substr(0,value.length()-1) << "state,predX,predY" << endl;
	while (file.good())
	{
	     getline (file, value, ',' ); 
	     string ts = value; 
	     getline (file, value, ',' );
	     double x = stod(value);
	     getline (file, value, ',' );
	     double y = stod(value);
	     getline (file, value, '\n' );
	     string s = value.substr(0,value.length()-1);
	     InputEvent e = predict(ts, x, y, s);
	     fileO << ts << "," << x << "," << y << "," << s << "," << e.x << "," << e.y << endl;
	     //cout << "{x:" << x << ", y:" << y << ", s: \"" << s << "\", xp:" << e.x << ", yp:" << e.y << "}, "; 
	}
	cout << endl;
	fileO.close();
	file.close();
}