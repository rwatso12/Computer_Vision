a = imaqhwinfo;
[camera_name, camera_id, format] = getCameraInfo(a);


% Capture the video frames using the videoinput function
% You have to replace the resolution & your installed adaptor name.
vid = videoinput(camera_name, camera_id, format);

% Set the properties of the video object
set(vid, 'FramesPerTrigger', Inf);
set(vid, 'ReturnedColorspace', 'rgb')
vid.FrameGrabInterval = 5;

%start the video aquisition here
start(vid)

while(vid.FramesAcquired<=20000)
    
data = getsnapshot(vid);
data_gray = rgb2gray(getsnapshot(vid));

 %subtract the red component 
    % from the grayscale image to extract the red components in the image.
    diff_im = imsubtract(data(:,:,1), rgb2gray(data));
    %Use a median filter to filter out noise
    diff_im = medfilt2(diff_im, [3 3]);
    % Convert the resulting grayscale image into a binary image.
    diff_im = im2bw(diff_im,0.18);
    
    % Remove all those pixels less than 300px
    diff_im = bwareaopen(diff_im,300);
    
    % Label all the connected components in the image.
    bw = bwlabel(diff_im, 8);
    
    % blob analysis.
    % properties for each labeled region.
    stats = regionprops(bw, 'BoundingBox', 'Centroid');
    MaskMatrix = ones(length(data_gray(:,1)),length(data_gray(1,:)));
    NegativeMatrix = xor(diff_im,MaskMatrix);
    NegativeMatrix = uint8(NegativeMatrix);
    
    % Display the img
    figure
    imshow(data)
    hold on
    %This is a loop to bound the red objects in a rectangular box.
    for object = 1:length(stats)
        bb = stats(object).BoundingBox;
        bc = stats(object).Centroid;
        rectangle('Position',bb,'EdgeColor','b','LineWidth',5) 
        plot(bc(1),bc(2), '-m+')
        crop=imcrop(data_gray,bb);
        Bar = crop(round(length(crop(:,1))/2),:);
        Bar_1=Bar;
        for i=1:length(Bar)
            if Bar(i)>=35
                Bar(i)=255;
            else
                Bar(i)=0;
            end
        end
        
       hold off
       
        Bar = find(Bar == 0);       
        DATA=Bar;
        DATA_1 = DATA;
        step = DATA(1)-1;

        for i = 1:length(DATA)
        DATA(i)=DATA(i)/(i+(step));
        if DATA(i)~=1
        Length_1 = i-1;
        break
        end
        end

        
        Step = step + DATA_1(Length_1+1)-DATA_1(Length_1);
        
        for i = (Length_1+1):length(DATA)
            DATA(i)=DATA_1(i+1)/(i+Step);
            if DATA(i)~=1
                Length_2 = i-1-Length_1;
                break
            end
        end
        
        Step_2 = DATA_1(Length_1+Length_2+2)-DATA_1(Length_1+Length_2+1);
        
           for i = Length_1+Length_2+2:length(DATA)
            DATA(i)=DATA(i+2)/(i+Step_2+Step);
            if DATA(i)~=1
                Length_3 = i-1-Length_1-Length_2;
                break
            end
           end
            
           Length_4=length(DATA)-Length_3-Length_2-Length_1-1;
        
        
    end
    hold off
    
    
    if Length_1 > Length_2 && Length_3 > Length_4 && Length_1 > Length_4 ...
            && Length_2 > Length_4
        
        a=1;
        figure
        imshow(crop)
        title('This is Bar Code 1') 
    end
    
    if Length_1 < Length_2 && Length_3 > Length_4 && Length_1 < Length_4 ...
            && Length_2 > Length_4
    
        a=2;
        figure
        imshow(crop)
        title('This is Bar Code 2') 
    end
    

    if Length_1 > Length_2 && Length_3 < Length_4 && Length_1 < Length_4 ...
            && Length_2 < Length_4
    
        a=3;
        figure
         imshow(crop)
        title('This is Bar Code 3') 
    end
    
        if Length_1 < Length_2 && Length_3 < Length_4 && Length_1 < Length_4 ...
            && Length_2 > Length_4
        
        a=4;
        figure
        imshow(crop)
        title('This is Bar Code 4') 
        end
    
end
   