<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="12008004">
	<Property Name="NI.LV.All.SourceOnly" Type="Bool">true</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="typedef" Type="Folder">
			<Item Name="Clear mode.ctl" Type="VI" URL="../Clear mode.ctl"/>
			<Item Name="Shutter mode.ctl" Type="VI" URL="../Shutter mode.ctl"/>
		</Item>
		<Item Name="Error translator.vi" Type="VI" URL="../Error translator.vi"/>
		<Item Name="Get frame size.vi" Type="VI" URL="../Get frame size.vi"/>
		<Item Name="Open camera.vi" Type="VI" URL="../Open camera.vi"/>
		<Item Name="Set clear and shutter.vi" Type="VI" URL="../Set clear and shutter.vi"/>
		<Item Name="Set gain and speed.vi" Type="VI" URL="../Set gain and speed.vi"/>
		<Item Name="Start acquisition.vi" Type="VI" URL="../Start acquisition.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="Cascade wrapper.dll" Type="Document" URL="../Visual Studio/Cascade wrapper/Release/Cascade wrapper.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
