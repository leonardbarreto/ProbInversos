function[z]=fx(x,y)
z=((x-1)^2)+((y-1)^2)-x*y
endfunction
x=[­6:0.1:6];
y=x;
w=feval(x,y,fx);
plot3d(x,x,w)