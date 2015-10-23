*************************************************************************
***      El energético problema de optimización                       ***
***                                                                   ***
*
*************************************************************************

* los mienbros de los sets están en el orden dado en el enunciado del problema
* e.g. f1 = "térmicas de carbón"
Sets
  i      conjunto de estados  /e1*e5/
  j      fuentes de energía   /carbon, hidro, eolica/;

* Utilizar m como alias para i
Alias(i, m);

*f1= termicas de carbon, hidroelectricas, energia eolica
*e1 = Oregon, e2= Washington, e3=California, e4=Arizona, e5=Nevada
Table c(i,j) matriz de costos en millones de dolares de producción de 1MWh en estado i con fuente j
         carbon      hidro   eolica
e1       18          17      11
e2       24          14      27
e3       9           22      23
e4       12          15      13
e5       19          11      24;

Parameter p(i)   requerimiento energético del estado i en MWh
  /e1 471, e2 927, e3 2039, e4 1249, e5 386/;


Table k(i,m) matriz de costos en millones de dolares de transporte de 1MWh de estado i a estado m
         e1   e2    e3    e4    e5
e1       0    8     5     12    6
e2       8    0     10    22    18
e3       5    10    0     4     4
e4       12   22    4     0     2
e5       6    18    4     2     0;

Table l(i,j) límite de MWh de energía de tipo j que puede producir el estado i
         carbon  hidro   eolica
e1       360     2784    1568
e2       690     5410    3173
e3       9785    5017    5594
e4       3470    2784    6241
e5       340     1874    1654;

Parameter h huella de carbono producida por 1MWh de energía de tipo carbon
  /0.1/;

Variables
  x(i,j)     MWh de tipo j que produce el estado i
  y(i, m)    MWh que compra el estado i al estado m
  mu(i)       Multa por huella de carbono del pais i
  z          minimizacion
  ctotal            valor costo;
Positive Variable x
Positive Variable y;


Equations
FObjetivo                    Funcion Objetivo

restr1(i)                    necesidades de los estados

restr2(i,j)                  no se puede exceder el límite de energía

restr3                       el 30% de la energía debe ser renovable

consumoTotal                 consumo total;

FObjetivo      ..      z =e= sum((i,j), x(i,j) * c(i,j)) + sum((i,m), y(i,m) * k(i,m)) + sum(i, mu(i));

restr1(i)         ..      sum(j, x(i,j)) + sum(m, y(i,m)) - sum(m, y(m,i)) =g= p(i);

restr2(i,j)         ..      x(i,j) =l= l(i,j);

restr3         ..     sum((i,j)$(ord(j) ne 1), x(i,j)) =g= sum((i,j),x(i,j)) * 0.3;

consumoTotal    ..      ctotal =e= sum((i,j), x(i,j));

Model energetico /all/ ;

option nlp=COUENNE
Solve energetico using nlp minimizing z;

Display x.l, y.l, mu.l, z.l, ctotal.l;



