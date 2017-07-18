function patch_grab(theta)

    cat = imread('cat.jpeg');
    
    [yi,xi,ni] = size(cat);

    xsize=300;
    ysize=300;

    %new_cat = zeros(1000,1000,'uint8');
    xpos = round(xi/2);
    ypos = round(yi/2);
    
    theta = theta*(pi/180);
    
    for y = 1:yi
        for x = 1:xi
            tx = x;
            ty = y;
            
            tty = ty - ypos;
            ttx = tx - xpos;

            
            rtx = ttx * cos(theta) - tty * sin(theta);
            rty = ttx * sin(theta) + tty * cos(theta);
            rtx = round(rtx+0.5*xpos);
            rty = round(rty+0.5*ypos);
            
            if(rtx <= 0 || rty <= 0 || ty <= 0 || tx <= 0 || rtx >= xsize || rty >= ysize || tx >= xi || ty >= yi)
                continue;
            end
            new_cat(rty,rtx,1) = cat(ty,tx,1);
            new_cat(rty,rtx,2) = cat(ty,tx,2);
            new_cat(rty,rtx,3) = cat(ty,tx,3);

        end
    end
    imshow(new_cat);
end