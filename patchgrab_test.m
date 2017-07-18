x = [10, 20, 55];
y = [20, 30,55];

xsize = [300,300,300];
ysize = [200,200,200];

theta = [0, 0, 9];

workorder = struct('x', x, 'y', y, 'xsize', xsize, 'ysize', ysize, 'theta', theta);


frame = imread('cat.jpeg');
tic;
results = patchgrab(frame, workorder);
toc;