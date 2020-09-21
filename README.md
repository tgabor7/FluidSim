# FluidSim

## Rácsos gázautomata (Lattice Gas Automata)
A rácsos gázautomaták olyan sejtautomaták amiket folyadék áramlás szimulására használnak, a rács-Boltzman módszer elődje.

### HPP Model(Hardy, Pomeau and de Pazzis)
Az első és legegyszerűbb rácsos gázautomata model a HPP Model. A model a részecske mozgási irányait tartalmazza a rácsokban. Az algoritmus 2 lépésből áll, az áramlás és az ütközésből. Az áramlási szakasz során a részecskék a mozgási irányuk szerint a következő cellába jut. Ütközés akkor következik be ha ugyanabba a cellába ellentétes irányú részecskék érkeznek, ekkor az irányuk 90°-kal elfordul.

<div>
<figure>
<img src="fluid.gif" width="300" height="300">
<figcaption>Részecskék ütközéskor 90°-kal elfordulnak.</figcaption>
</figure> <img src="nothing.gif" width="300" height="300">
</div>
# References
[1]. https://en.wikipedia.org/wiki/Lattice_gas_automaton
