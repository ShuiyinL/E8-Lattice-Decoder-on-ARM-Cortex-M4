function y = CompressQ(x,q,d)
if 2^d < q
    y=mod(round(2^d/q*x),2^d);
else
    y=mod(x,q);
end

end

