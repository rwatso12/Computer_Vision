clear all
clc

I = imread('Picture 17.jpg');
Ibw = ~im2bw(I,graythresh(I));
Ifill = imfill(Ibw,'holes');
Iarea = bwareaopen(Ifill,100);
Ifinal = bwlabel(Iarea);
stat = regionprops(Ifinal,'boundingbox');
imshow(I); hold on;
for cnt = 1 : numel(stat)
    bb = stat(cnt).BoundingBox;
    rectangle('position',bb,'edgecolor','r','linewidth',2);
end