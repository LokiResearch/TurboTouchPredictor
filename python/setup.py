# Flavien Volant and Géry Casiez

import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
     name='TurboTouchPredictor',  
     version='0.1.5',
     author="Flavien Volant and Géry Casiez",
     author_email="gery.casiez@univ-lille.fr",
     description="TurboTouch predictor",
     install_requires = ["OneEuroFilter"],
     long_description=long_description,

     long_description_content_type="text/markdown",
     url="https://github.com/LokiResearch/TurboTouchPredictor/python",
     packages=setuptools.find_packages(),

     classifiers=[
         "Programming Language :: Python :: 3",
         "License :: OSI Approved :: BSD License"
     ],

 )