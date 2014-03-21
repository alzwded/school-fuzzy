school-fuzzy
============

fuzzy logic assignment(s)

Assignment 1
------------

* Implement Left, Right, Triangle, Trapezium Saturation functions
* Let x [-2, 2], dx [-1, 1] be variables defined for
  - NE-L(-1, -0.5), ZE-TRAP(-1, -0.5, 0.5, 1), PO-R(0.5, 1)
  - NE-L(-0.5, -0.25), ZE-TRIA(-0.5, 0, 0.5), PO-R(0.25, 0.5)
* compute the values for (x = 0.6, dx = -0.4)

Assignment 2
------------

* Compute inference matrix
* Compose results according to a rules matrix
| dx\x | NE | ZE | PO |
|------|----|----|----|
| NE   | PO | ZE | ZE |
| ZE   | PO | ZE | NE |
| PO   | ZE | NE | NE |
