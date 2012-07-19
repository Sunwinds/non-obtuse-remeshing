function Q = centroidToQuadric(centroid)
%Hui Wang, July 17, 2012

Q = eye(4);

Q(1, 4) = -1.0 * centroid(1);
Q(2, 4) = -1.0 * centroid(2);
Q(3, 4) = -1.0 * centroid(3);

Q(4, 1) = -1.0 * centroid(1);
Q(4, 2) = -1.0 * centroid(2);
Q(4, 3) = -1.0 * centroid(3);

Q(4, 4) = sum(centroid .^ 2);