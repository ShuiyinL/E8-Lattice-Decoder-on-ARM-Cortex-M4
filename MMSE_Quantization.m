clear all
close all

%%%%% Generate the dataset: Cu9-11.mat Cv3-6.mat
%d_x: d_v or d_u
d_x=11;
q=3329;
len=2^d_x;

training_set = 0:q-1;

%%%%%% Original Kyber Compression noise distribuion
x=CompressQ(training_set,q,d_x);
% %fix the boundary error, e.g., mod(round(2^10/3329*3328),2^10)=0
% x(x==0)=2^d_x; 
% x(1:2^(11-d_x))=0;
Ll_err=training_set-DecompressQ(x,q,d_x);
Ll_range=unique(Ll_err);
pmf_Ll=zeros(1,length(Ll_range));
%%%% distribution of original kyber compression error 
for ii=1:1:length(Ll_range)
    pmf_Ll(ii)=sum(Ll_err==Ll_range(ii))/q;
end
fprintf('Original Kyber Compression Noise Distribution, d = %d \n', d_x)
kyber_compression_noise_space=Ll_range
kyber_compression_noise_probability=pmf_Ll
var_kyber_compression_noise=Ll_range.^2*pmf_Ll'


%%%%%% MMSE quantizatin noise distribuion
%% Use to generate (LloydCu, LloydCv), e.g., Cu_dx.mat Cv_dx.mat
%Original Kyber compression noise has larger variance than the one of MMSE
%quantiztion: i.e., var_kyber_compression_noise > var_MMSE_quantization_noise

[partition,codebook] = lloyds(training_set,len);
[index,quants] = quantiz(training_set,partition,codebook);
% mse_lloyds=(norm(training_set-quants)^2)/length(training_set)

Ll_err=(training_set-quants);
Ll_range=unique(Ll_err);
pmf_Ll=zeros(1,length(Ll_range));
%%%% distribution of MMSE quantizatin error 
for ii=1:1:length(Ll_range)
    pmf_Ll(ii)=sum(Ll_err==Ll_range(ii))/q;
end
fprintf('MMSE Quantizatin Noise Distribution, d = %d \n', d_x)
MMSE_quantization_noise_space=Ll_range
MMSE_quantization_noise_probability=pmf_Ll
var_MMSE_quantization_noise=Ll_range.^2*pmf_Ll'
%save Cudx.mat  pmf_Ll Ll_range


