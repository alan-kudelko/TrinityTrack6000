description=["Injora 540 13T 1:50","Injora 540 13T 1:139","Injora 540 13T 1:189","Injora 540 13T 1:263","Injora 540 13T 1:516","Injora 540 13T 1:721"];
rpm_max=[32500/50,32500/139,32500/189,32500/263,32500/516,32500/721]; %rpm
current_rpm_max=[6.8,6.8,6.8,6.8,6.8,6.8]; %A
current_rpm_stall=[170,170,170,170,170,170];
rpm_max_radians=rpm_max./60*2*pi;
V=7.4; %V

% V=kw+IR+LI'
% w=V/k-Rt/(k^2)
% k=V/w
% ks=(k+V)/R
R=V./current_rpm_stall;
kt=V./rpm_max_radians;
ks=(kt*V)./R;
figure('units','inch','position',[0.25 -0.25 10 9]);
subplot(3,1,1)
hold all;
grid on;

[rpm_plot,Torque_plot]=gen_data(rpm_max_radians,R,kt,V);
for i=1:1:length(rpm_max)
    plot(rpm_plot(i,:)/2/pi*60,Torque_plot(i,:));
end

title("Torque vs rpm",'FontSize',15);
xlabel("rpm",'FontSize',14);
ylabel("Torque [Nm]",'FontSize',14)
xlim([0 1000])
legend(description,'FontSize',14)
xlim([0,700]);
subplot(3,1,2);
hold all;
grid on;

[rpm_plot,Power_plot]=gen_data_power(rpm_max_radians,R,Torque_plot,V);
for i=1:1:length(rpm_max)
    plot(rpm_plot(i,:)/2/pi*60,Power_plot(i,:));
end

title("Power vs rpm",'FontSize',15);
xlabel("rpm",'FontSize',14);
ylabel("Power [W]",'FontSize',14)
xlim([0,700]);
xlim([0,650]);
subplot(3,1,3);
hold all;
grid on;
[current_plot,efficiency_plot]=get_data_eff(rpm_plot,rpm_max_radians,current_rpm_max,current_rpm_stall,Torque_plot,V);
for i=1:1:length(rpm_max)
    plot(rpm_plot(i,:)/2/pi*60,efficiency_plot(i,:));
end

title("Efficiency vs rpm",'FontSize',15);
xlabel("rpm",'FontSize',14);
ylabel("Efficiency [%]",'FontSize',14)
xlim([0,700]);
xlim([0,650]);
function [rpm_plot,Torque_plot]=gen_data(rpm_max_radians,R,kt,V)
    N=length(rpm_max_radians);
    Torque_plot=repmat([linspace(0,0,1000)],N,1);
    rpm_plot=repmat([linspace(0,0,1000)],N,1);
    for i=1:1:length(rpm_max_radians)
        rpm_plot(i,:)=linspace(0,rpm_max_radians(i),1000);
        Torque_plot(i,:)=(-rpm_plot(i,:)*kt(i)^2+V*kt(i))/R(i);
    end
end
function [rpm_plot,Power_plot]=gen_data_power(rpm_max_radians,R,Torque_plot,V)
    N=length(rpm_max_radians);
    Power_plot=repmat([linspace(0,0,1000)],N,1);
    rpm_plot=repmat([linspace(0,0,1000)],N,1);
    for i=1:1:length(rpm_max_radians)
        rpm_plot(i,:)=linspace(0,rpm_max_radians(i),1000);
        Power_plot(i,:)=rpm_plot(i,:).*Torque_plot(i,:);
    end
end
function [current_plot,efficiency_plot]=get_data_eff(rpm_plot,rpm_max_radians,current_rpm_max,current_rpm_stall,Torque_plot,V)
    N=length(rpm_max_radians);
    efficiency_plot=repmat([linspace(0,0,1000)],N,1);
    current_plot=repmat([linspace(0,0,1000)],N,1);
    for i=1:1:length(rpm_max_radians)
        current_plot(i,:)=linspace(current_rpm_stall(i),current_rpm_max(i),1000);
        efficiency_plot(i,:)=rpm_plot(i,:).*Torque_plot(i,:)./current_plot(i,:)/V;
    end
end