t1 = [0:0.01:50];
t2 = [0:0.01:50];
while(1)
    t1 = t1 - 1;
    t2 = t2 - 0.3;
    v1 = sin(t1);
    v2 = sin(0.07*t2);
    vm = v1.*v2;
    plot(vm);
    pause(0.05);
end
