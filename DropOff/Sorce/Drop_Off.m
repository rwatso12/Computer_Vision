clear all
clc

    % Get the snapshot of the current frame
    data = imread('Picture 18.jpg');
    data_gray = rgb2gray(imread('Picture 18.jpg'));

    Channel_1 = data(:,:,1)+data(:,:,2);
    Blue = data(:,:,3)-Channel_1;

    figure
    imshow(Blue)

    
    BW = im2bw(Blue,.05);
    % Remove all those pixels less than 50px
    BW = bwareaopen(BW,50);
    %se = strel('square',100);
    se = strel('disk',4);
    dilatedBW = imdilate(BW,se);
    figure, imshow(dilatedBW)
    BW1 = imdilate(BW,se);

    % blob analysis.
    % properties for each labeled region.
    stats = regionprops(BW1, 'BoundingBox', 'Centroid');
    
%     figure
%     imshow(BW1)
 
    %     % Display the img
    figure
    imshow(data)
    hold on
    %This is a loop to bound the red objects in a rectangular box.
    for object = 1:length(stats)
        bb = stats(object).BoundingBox;
        bc = stats(object).Centroid;
        rectangle('Position',bb,'EdgeColor','b','LineWidth',5) 
        plot(bc(1),bc(2), '-m+')
    end
     hold off
%      
%         subImage = imcrop(BW, bb);
% 
        [H,theta,rho] = hough(BW);

        P = houghpeaks(H,5,'threshold',ceil(0.3*max(H(:))));
        
        x = theta(P(:,2));
        y = rho(P(:,1));
        
        lines = houghlines(BW,theta,rho,P,'FillGap',50,'MinLength',7);
        
        figure, imshow(BW), hold on
        max_len = 0;
        for k = 1:length(lines)
           xy = [lines(k).point1; lines(k).point2];
           plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');

           % Plot beginnings and ends of lines
           plot(xy(1,1),xy(1,2),'x','LineWidth',5,'Color','yellow');
           plot(xy(2,1),xy(2,2),'x','LineWidth',5,'Color','red');

           % Determine the endpoints of the longest line segment
           len = norm(lines(k).point1 - lines(k).point2);
           if ( len > max_len)
              max_len = len;
              xy_long = xy;
           end
        end
                plot(xy_long(:,1),xy_long(:,2),'LineWidth',2,'Color','red');

     