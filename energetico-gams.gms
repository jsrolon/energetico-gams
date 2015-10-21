*************************************************************************
***      El energético problema de optimización                       ***
***                                                                   ***
*
*************************************************************************

* los mienbros de los sets están en el orden dado en el enunciado del problema
* e.g. f1 = "térmicas de carbón"
Sets
  i      conjunto de estados  /e1*e5/
  j      fuentes de energía   /f1*f3/;

* Utilizar m como alias para i
Alias(i, m);

*f1= termicas de carbon, hidroelectricas, energia eolica
*e1 = Oregon, e2= Washington, e3=California, e4=Arizona, e5=Nevada
Table c(i,j) matriz de costos de producción de 1MWh en estado i con fuente j
      f1    f2    f3
   e1  1     1     1
   e2  1     1     1
   e3  1     1     1
   e4  1     1     1
   e5  1     1     1;

Parameter p(i)   requerimiento energético del estado i en MWh
  /e1 6000, e2 7000, e3 8000, e4 9500, e5 11000/;

Table k(i,m) matriz de costos de transporte de 1MWh de estado i a estado m
         e1   e2    e3    e4    e5
e1       1    1     1     1      1
e2       1    1     1     1      1
e3       1    1     1     1      1
e4       1    1     1     1      1
e5       1    1     1     1      1;

Table l(i,j) límite de MWh de energía de tipo j que puede generar  el estado i
         f1    f2    f3
e1       1     1     1
e2       1     1     1
e3       1     1     1
e4       1     1     1
e5       1     1     1;

Parameter h(j)   huella de carbón producida por 1MWh de energía de tipo j
  /f1 6000, f2 7000, f3 8000/;

* falta agregar la funcion m(h)

Variables
  x(i,j)     MWh de tipo j que produce el estado i
  y(i, m)    MWh que compra el estado i al estado m
  z          minimizacion;
Positive Variable x
Positive Variable y;


Equations
FObjetivo                    Funcion Objetivo

restr1(i)                    necesidades de los estados

restr2(i,j)                  no se puede exceder el límite de energía

restr3                       el 30% de la energía debe ser renovable;

FObjetivo      ..      z =e= sum((i,j), x(i,j) * c(i,j)) + sum((i,m), y(i,m) * k(i,m)) + sum((i,j), x(i,j) * h(j));

restr1(i)         ..      sum(j, x(i,j)) + sum(m, y(i,m)) =g= p(i);

restr2(i,j)         ..      x(i,j) =l= l(i,j);

restr3         ..     sum((i,j)$(ord(j) ne 1),x(i,j)) =g= sum((i,j),x(i,j)) * 0.3;

Model energetico /all/ ;

option mip=CPLEX
Solve energetico using mip minimizing z;

Display x.l;
Display y.l;
Display z.l;



