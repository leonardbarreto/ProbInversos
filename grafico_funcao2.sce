function[z]=fx(x,y)
z=(x^2)+3*(y^2)-2*x+2*y
endfunction
x=[­2:0.1:1.5];
y=x;
w=feval(x,y,fx);
plot3d(x,x,w)