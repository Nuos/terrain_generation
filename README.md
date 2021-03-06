# Procedural terrain generation

Here I will put procedural terrain generation experiments

### Fluid inspired terrain generation

#### 2016.04.02
It seems to be working to some degree, however it should be forming phases faster to be an efficient way of making an interesting terrain. 

![2016.04.02.gif](fluid_terrain/screenshot/2016.04.02.gif)

#### 2016.04.01
I have tried adding a potential function. I have also tested out some kind of simulated annealing, allowing the particles to moving longer distances at the start, then lowering the "temperature". It is not stable, but it is showing some forming of phases.

![2016.04.01.gif](fluid_terrain/screenshot/2016.04.01.gif)

#### 2016.03.29
The thought here is to make the terrain cells behave as particles, and do a Monte-Carlo-inspired simulation, thinking that the particles will create different phases. However, this does not work properly yet, as can be seen in the animation below. 

The terain cell "moves" a randomly chosen direction if that results in having a higher number of neighbours of the same type. This I will change to calculating some kind of energy. The interaction energy between equal terrain will be low, and interaction energy between unequal terrain will be high. Each step in the simulation will then only be accepted if the energy change is negative, or if the increase is sufficiently low. 

![2016.03.29.gif](fluid_terrain/screenshot/2016.03.29.gif)
