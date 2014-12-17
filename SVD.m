% Read the picture of the tiger, and convert to black and white.
barcode = rgb2gray(imread('Ansel_Adams.jpg'));


% Downsample, just to avoid dealing with high-res images.
barcode = im2double(imresize(barcode, 0.5));

% Compute SVD of this tiger
[U, S, V] = svd(barcode);

% Plot the magnitude of the singular values 
sigmas = diag(S);
figure; plot(sigmas); title('Singular Values');
figure; plot(cumsum(sigmas) / sum(sigmas)); title('Cumulative Percent of Total Sigmas');

% Show full-rank img
figure;
imshow(barcode)
title('Full-Rank Img');

% Compute low-rank approximations of the img
ranks = [200, 100, 50, 30, 20, 10, 3];
for i = 1:length(ranks)
    % Keep largest singular values, and nullify others.
    approx_sigmas = sigmas; approx_sigmas(ranks(i):end) = 0;

    % Form the singular value matrix, padded as necessary
    ns = length(sigmas);
    approx_S = S; approx_S(1:ns, 1:ns) = diag(approx_sigmas);

    % Compute low-rank approximation by multiplying out component matrices.
    approx_img = U * approx_S * V';

    % Plot approximation
    figure
    imshow(approx_img)
    title(sprintf('Rank %d Img', ranks(i)));
    
end