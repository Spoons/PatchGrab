x = int32([10, 20,55,700,300,300,300,300]);
y = int32([20, 30,55,525,300,300,300,300]);

xsize = int32([300,300,300,300,300,300,300,300]);
ysize = int32([200,200,200,300,300,300,300,300]);

theta = int32([0, 0, 9,90,300,300,300,300]);

frame = imread('cat.jpeg');
workorder = struct('frame', frame, 'x', x, 'y', y, 'xsize', xsize, 'ysize', ysize, 'theta', theta);

tic;
output = patchgrab(workorder);
t2=toc;
%imshow(output(4).patch)

fprintf('Time per patch: %f\n',(t2)/8);