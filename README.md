
# FluidSim

## Rácsos gázautomata (Lattice Gas Automata)
A rácsos gázautomaták olyan sejtautomaták amiket folyadék áramlás szimulására használnak, a rács-Boltzman módszer elődje.

### HPP Model(Hardy, Pomeau and de Pazzis)
Az első és legegyszerűbb rácsos gázautomata model a HPP Model. A model a részecske mozgási irányait tartalmazza a rácsokban. Az algoritmus 2 lépésből áll, az áramlás és az ütközésből. Az áramlási szakasz során a részecskék a mozgási irányuk szerint a következő cellába jut. Ütközés akkor következik be ha ugyanabba a cellába ellentétes irányú részecskék érkeznek, ekkor az irányuk 90°-kal elfordul. A model diszkrét áramlási sebességei a következők:
e<sub>0</sub>=(1,0)
e<sub>1</sub>=(0,1)
e<sub>2</sub>=(-1,0)
e<sub>3</sub>=(0,-1)

Egy 500x500-as rácsot 4 áramlási sebességgel a következőképpen tudjuk ábrázolni.:

f[x][y][i] = b (1 vagy 0)

ahol f egy három dimenziós boolean tömb, x a rács x(0-499), y a rács y(4-499) koordinátája. Az i(0-3) index a fentebb definiált sebességek indexe. Más szóval a fenti képlet megadja hogy az (x,y) helyen van-e e<sub>i</sub> irányú részecske.

Ezután az ütközési szakasz a következő képpen írhatő le.:

f<sup>'</sup>[x][y][i] = f[x][y][i] + c

, ahol a c értéke:<br>

<p>c = f[x][y][(i+1)%4]*(1-f[x][y][i])*(1-f[x][y][(i+2)%4])-(1-f[x][y][(i+1)%4])*(1-f[x][y][(i+3)%4])*(f[x][y][i])*(f[x][y][(i+2)%4]) </p>

Példa kód a HPP Modelre:
```cpp
stream()
{
	for (int i = 1; i < width-1; i++) {
		for (int j = 1; j < height-1; j++) {
			for (int k = 0; k < 4; k++) {
				vector2 e = velocities[k];
				f[(i+(int)e.x)%width][(j+(int)e.y)%height][k] = fn[i][j][k];
			}
		}
	}
}
collide()
{
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			for (int k = 0; k < 4; k++) {
				int c = f[i][j][(k + 1) % 4] * f[i][j][(k + 3) % 4] * (1 - f[i][j][k]) * (1 - f[i][j][(k + 2) % 4]) -
					(1 - f[i][j][(k + 1) % 4]) * (1 - f[i][j][(k + 3) % 4]) * f[i][j][k] * f[i][j][(k + 2) % 4];
				fn[i][j][k] = f[i][j][k] + c;
			}
		}
	}
}
```

| <img src="images/fluid.gif" width="300" height="300"> | <img src="images/nothing.gif" width="300" height="300"> | <img src="images/bounce.gif" width="300" height="300"> |
|:--:| :--: | :--: |
| *Ellenkező irányú részecskék ütközése* | *90°-os ütközés* | *Fallal való ütközés* |

### FHP Model(Frisch, Hasslacher, and Pomeau)
Az FHP és HPP Model között a legnagyobb különbség a részecskék áramlási irányainak száma, ami az FHP esetében 6.

| <img src="images/fhp.gif" width="300" height="300"> | <img src="images/fhpcollision.gif" width="300" height="300">|
| :--: | :--: |
| Három részecske ütközés az FHP Modelben | Két részecske ütközése az FHP Modelben | 
### LBM (Rács Boltzmann Módszer)
A rács Bolltzman módszernél a rács pontok eloszlási függvényeket tartalmaznak.
##LBGK Model
Az LBGK modelleket dimenzió(n) és sebesség(b) szám által csoporotsítják (DnQb).A következőkben a D2Q9 modellel foglalkozunk. Az áramlás nem változik az LGA-hoz képest, azonban az ütközési szakasz kicsit összetettebb.
i a sebességek száma
Az eloszlási függvény a következő tömbbel ábrázolható.:
f[x][y][i] = R (valós szám)
Az algoritmushoz tudnunk kell az egyes rácspontok sűrűségét és a részecskék sebességét, amiket a következő tömbben tárolunk.:
u[x][y] = R^2
ρ[x][y] = R

f<sup>'</sup>[x][y][i] = f[x][y][i] + Ω
ahol,
Ω = -(f[x][y][i] - feq[ρ[x][y]][u[x][y]][i]) / τ
és
feq[ρ][u][i] = ρ * (A+B*(e[i]*u)+C*u^2+D*(e[i]*u)^2)
<img src="https://render.githubusercontent.com/render/math?math=\Large feq[\rho][u][i]=\rho * (A + B*(e[i]*u)+C*u^2 + D*(e[i]*u)^2)">


- τ relaxációs idő
- ρ = sum fi rács sűrűség
- u = sum (fi*ei)/rho rács sebesség

Az áramlás ábrázolásakor a sebességek abszolút értékét rajzoljuk ki.

`code code code`

### CUDA program
Legköltségesebb az adatok memóriába másolása
### Reynolds szám
<img src="https://render.githubusercontent.com/render/math?math=\Large Re=\frac{\rho uL}{\mu}">

- ρ a folyadék sűrűsége
- u az áramlási sebesség
- L a jellemző hosszméret
- μ a dinamikai viszkozitás

# References
[1]. https://en.wikipedia.org/wiki/Lattice_gas_automaton <br>
[2]. Lattice Boltzmann Method and Its Applications in Engineering(Zhaoli Guo, Chang Shu)
