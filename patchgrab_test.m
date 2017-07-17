x = [10, 20, 55];
y = [20, 30,55];
theta = [0, 0, 9];

workorder = struct('x', x, 'y', y, 'theta', theta);


frame = imread('cat.jpeg');
tic;
patchgrab(frame, workorder);
toc;