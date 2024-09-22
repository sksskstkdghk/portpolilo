START ../../PacketGenerator/bin/Debug/PacketGenerator.exe ../../PacketGenerator/PDL.xml

xcopy /Y GenPackets.cs "../../DummyClient/Packet"
xcopy /Y GenPackets.cs "C:/Users/sanghwa/Desktop/PortPolilo/Unity_PortPolilo/Assets/Scripts/Client/Packet"
xcopy /Y GenPackets.cs "../../Server/Packet"

xcopy /Y ClientPacketManager.cs "../../DummyClient/Packet"
xcopy /Y ClientPacketManager.cs "C:/Users/sanghwa/Desktop/PortPolilo/Unity_PortPolilo/Assets/Scripts/Client/Packet"
xcopy /Y ServerPacketManager.cs "../../Server/Packet"