x = [10, 20];
y = [20, 30];
theta = [0, 0];

workorder = struct('x', x, 'y', y, 'theta', theta);


frame = imread('cat.jpeg');
patchgrab(frame);
