# joby_sim

This is a pass at a simple solution/implementation for the eVTOL Simulation Problem from JOBY (see the file eVtol_Simulation_Problem.pdf)

## Things to note
### This is a cmake based project
This should work to build:
```
> mkdir build
> cd build
> cmake -DCMAKE_BUILD_TYPE=Debug ../
> make
```
Then to run the simulation:
```
>./joby_simulation
```

### google test unit testing
I created a couple of simple unit tests and included googletest as a git submodule. 
I'm new to google tests (I usually use catch2) so I hope this works for you.

The tests should be created by the `> make` above. To run the unit tests:
From the `build` directory:
```
> ./googletest/run_tests
```
