START ../../PacketGenerator/bin/Release/PacketGenerator.exe ../../PacketGenerator/PDL.xml

xcopy /Y GenPackets.cs "../../DummyClient/Packet"
xcopy /Y GenPackets.cs "../../Server/Packet"

xcopy /Y PacketManager.cs "../../DummyClient/Packet"
xcopy /Y PacketManager.cs "../../Server/Packet"