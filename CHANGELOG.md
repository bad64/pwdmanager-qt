**FEATURES**

- All: Added support for several command line args, which aren't 
documented yet (but a quick look at src/main.cpp should be enough)
- All: Added a function to merge two databases together, eliminating (presumed) duplicate entries (basically, if username && purpose && password all match, it's considered a duplicate, so it's pretty lenient)