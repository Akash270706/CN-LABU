#include "ns3/core-module.h" 
#include "ns3/network-module.h" 
#include "ns3/internet-module.h" 
#include "ns3/point-to-point-module.h" 
#include "ns3/applications-module.h" 
#include "ns3/netanim-module.h" 
using namespace ns3; 
int main(int argc, char *argv[]) 
{ 
NodeContainer nodes; 
nodes.Create(2); 

PointToPointHelper p2p; 
p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps")); 
p2p.SetChannelAttribute("Delay", StringValue("2ms")); 
NetDeviceContainer devices; 
devices = p2p.Install(nodes); 

InternetStackHelper stack; 
stack.Install(nodes); 

Ipv4AddressHelper address; 
address.SetBase("10.1.1.0", "255.255.255.0"); 
Ipv4InterfaceContainer interfaces; 
interfaces = address.Assign(devices); 

uint16_t port = 5000; 
PacketSinkHelper server( 
"ns3::TcpSocketFactory", 
InetSocketAddress(Ipv4Address::GetAny(), port)); 
ApplicationContainer serverApp; 
serverApp = server.Install(nodes.Get(1)); 
serverApp.Start(Seconds(1.0)); 
serverApp.Stop(Seconds(10.0)); 

BulkSendHelper client( 
"ns3::TcpSocketFactory", 
InetSocketAddress(interfaces.GetAddress(1), port)); 
client.SetAttribute("MaxBytes", UintegerValue(0)); 
ApplicationContainer clientApp; 
clientApp = client.Install(nodes.Get(0)); 
clientApp.Start(Seconds(2.0)); 
clientApp.Stop(Seconds(10.0)); 

AsciiTraceHelper ascii; 
p2p.EnableAsciiAll( 
ascii.CreateFileStream("tcp.tr")); 

AnimationInterface anim("tcp.xml"); 
anim.SetConstantPosition(nodes.Get(0), 0, 0); 
anim.SetConstantPosition(nodes.Get(1), 50, 0); 

Simulator::Run(); 
Simulator::Destroy(); 
return 0; 
}
