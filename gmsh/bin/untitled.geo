//+
SetFactory("Built-in");
//+
SetFactory("Built-in");
//+
Point(1) = {0, 0, 0, 1.0};
//+
Point(2) = {100, 0, 0, 1.0};
//+
Point(3) = {100, 100, 0, 1.0};
//+
Point(4) = {0, 100, 0, 1.0};
//+
Line(1) = {4, 3};
//+
Line(2) = {3, 2};
//+
Line(3) = {2, 1};
//+
Line(4) = {1, 4};
//+
SetFactory("OpenCASCADE");
Circle(5) = {50, 50, 0, 15, 0, 2*Pi};
//+
Curve Loop(1) = {4, 1, 2, 3};
//+
Curve Loop(1) = {5};
//+
Plane Surface(1) = {1};
//+
SetFactory("Built-in");
//+
SetFactory("Built-in");
//+
SetFactory("Built-in");
//+
SetFactory("Built-in");
//+
Curve Loop(2) = {4, 1, 2, 3};
//+
Curve Loop(3) = {5};
//+
Plane Surface(2) = {2, 3};
