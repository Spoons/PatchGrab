function patch=patch_grab(x,y,theta)

    cat = imread('cat.jpeg');
    
    [yi,xi,ni] = size(cat);

    xsize=300;
    ysize=300;

    new_cat = zeros(ysize,xsize,'uint8');
    xpos = round(x);
    ypos = round(y);
    
    
    theta = theta*(pi/180);
    
    yhalf = round((1/2)*ysize); 
    xhalf = round((1/2)*xsize);
    
    xposhalf = round(xpos*(1/2));
    yposhalf = round(ypos*(1/2));
    
    bound_hit = 0;
    pix_hit = 0;
    for y = ypos-yhalf:ypos+yhalf
        for x = xpos-xhalf:xpos+xhalf
            tx = x;
            ty = y;
            
            ydif = ypos - y;
            xdif = xpos - x;
            
            rtx = xpos +(-xdif * cos(theta) - ydif * sin(theta));
            rty = ypos + (-ydif * cos(theta) + xdif * sin(theta));

            
            rtx = round(rtx);
            rty = round(rty);
            
            tx = tx - xpos + xhalf; 
            ty = ty - ypos + yhalf;


            if(rtx <= 0 || rty <= 0 || ty <= 0 || tx <= 0 || rtx >= xi || rty >= yi || tx > xsize || ty > ysize)
                bound_hit = bound_hit + 1;
                continue;
           end
            
            new_cat(ty,tx,1) = cat(rty,rtx,1);
            new_cat(ty,tx,2) = cat(rty,rtx,2);
            new_cat(ty,tx,3) = cat(rty,rtx,3);
            
            pix_hit = pix_hit + 1;

        end
    end
    imshow(new_cat);
    fprintf("boundary hit: %i\npix hit: %i\n", bound_hit, pix_hit);
    patch = new_cat;
end