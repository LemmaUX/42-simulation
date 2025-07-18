"""
OSI Layers Module for Packet Odyssey
Handles packet encapsulation and visualization across the 7 OSI layers
"""

from rich.console import Console
from rich.panel import Panel
from rich.table import Table
from rich.text import Text
from rich import box
from scapy.all import *
import socket
import time
from typing import Dict, Any, Optional

console = Console()

class OSILayer:
    """Base class for OSI layer operations"""
    
    def __init__(self, layer_name: str, layer_number: int):
        self.layer_name = layer_name
        self.layer_number = layer_number
        self.data = {}
    
    def encapsulate(self, data: Dict[str, Any]) -> Dict[str, Any]:
        """Add layer-specific headers to the packet"""
        raise NotImplementedError
    
    def display(self, data: Dict[str, Any]) -> None:
        """Display layer information in a formatted way"""
        raise NotImplementedError

class ApplicationLayer(OSILayer):
    """Layer 7: Application Layer - HTTP, DNS, etc."""
    
    def __init__(self):
        super().__init__("Application", 7)
    
    def encapsulate(self, data: Dict[str, Any]) -> Dict[str, Any]:
        """Create HTTP request data"""
        http_data = {
            "method": "GET",
            "path": "/",
            "version": "HTTP/1.1",
            "headers": {
                "Host": data.get("host", "example.com"),
                "User-Agent": "PacketOdyssey/1.0",
                "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
                "Accept-Language": "en-US,en;q=0.5",
                "Accept-Encoding": "gzip, deflate",
                "Connection": "keep-alive"
            },
            "body": ""
        }
        
        data["application"] = http_data
        return data
    
    def display(self, data: Dict[str, Any]) -> None:
        """Display HTTP request details"""
        app_data = data.get("application", {})
        
        table = Table(title=f"🔧 Layer {self.layer_number}: {self.layer_name}", box=box.ROUNDED)
        table.add_column("Field", style="cyan", no_wrap=True)
        table.add_column("Value", style="green")
        
        table.add_row("Method", app_data.get("method", "GET"))
        table.add_row("Path", app_data.get("path", "/"))
        table.add_row("Version", app_data.get("version", "HTTP/1.1"))
        
        # Add headers
        headers = app_data.get("headers", {})
        for key, value in headers.items():
            table.add_row(f"Header: {key}", str(value))
        
        console.print(table)
        console.print()

class PresentationLayer(OSILayer):
    """Layer 6: Presentation Layer - Data formatting, encryption"""
    
    def __init__(self):
        super().__init__("Presentation", 6)
    
    def encapsulate(self, data: Dict[str, Any]) -> Dict[str, Any]:
        """Add presentation layer formatting"""
        presentation_data = {
            "encoding": "UTF-8",
            "compression": "gzip",
            "encryption": "TLS 1.3",
            "data_format": "JSON/XML"
        }
        
        data["presentation"] = presentation_data
        return data
    
    def display(self, data: Dict[str, Any]) -> None:
        """Display presentation layer details"""
        pres_data = data.get("presentation", {})
        
        table = Table(title=f"🎨 Layer {self.layer_number}: {self.layer_name}", box=box.ROUNDED)
        table.add_column("Field", style="cyan", no_wrap=True)
        table.add_column("Value", style="green")
        
        for key, value in pres_data.items():
            table.add_row(key.title(), str(value))
        
        console.print(table)
        console.print()

class SessionLayer(OSILayer):
    """Layer 5: Session Layer - Session management"""
    
    def __init__(self):
        super().__init__("Session", 5)
    
    def encapsulate(self, data: Dict[str, Any]) -> Dict[str, Any]:
        """Add session layer information"""
        session_data = {
            "session_id": f"session_{int(time.time())}",
            "authentication": "Bearer Token",
            "session_timeout": "30 minutes",
            "connection_type": "persistent"
        }
        
        data["session"] = session_data
        return data
    
    def display(self, data: Dict[str, Any]) -> None:
        """Display session layer details"""
        session_data = data.get("session", {})
        
        table = Table(title=f"🤝 Layer {self.layer_number}: {self.layer_name}", box=box.ROUNDED)
        table.add_column("Field", style="cyan", no_wrap=True)
        table.add_column("Value", style="green")
        
        for key, value in session_data.items():
            table.add_row(key.replace("_", " ").title(), str(value))
        
        console.print(table)
        console.print()

class TransportLayer(OSILayer):
    """Layer 4: Transport Layer - TCP/UDP"""
    
    def __init__(self):
        super().__init__("Transport", 4)
    
    def encapsulate(self, data: Dict[str, Any]) -> Dict[str, Any]:
        """Create TCP segment"""
        transport_data = {
            "protocol": "TCP",
            "source_port": 49152,  # Dynamic port
            "dest_port": 80,       # HTTP port
            "sequence_number": 1000,
            "ack_number": 0,
            "flags": {
                "SYN": True,
                "ACK": False,
                "FIN": False,
                "RST": False,
                "PSH": False,
                "URG": False
            },
            "window_size": 65535,
            "checksum": "0x1234"
        }
        
        data["transport"] = transport_data
        return data
    
    def display(self, data: Dict[str, Any]) -> None:
        """Display TCP segment details"""
        transport_data = data.get("transport", {})
        
        table = Table(title=f"🚢 Layer {self.layer_number}: {self.layer_name}", box=box.ROUNDED)
        table.add_column("Field", style="cyan", no_wrap=True)
        table.add_column("Value", style="green")
        
        table.add_row("Protocol", transport_data.get("protocol", "TCP"))
        table.add_row("Source Port", str(transport_data.get("source_port", 49152)))
        table.add_row("Dest Port", str(transport_data.get("dest_port", 80)))
        table.add_row("Seq Number", str(transport_data.get("sequence_number", 1000)))
        table.add_row("Ack Number", str(transport_data.get("ack_number", 0)))
        table.add_row("Window Size", str(transport_data.get("window_size", 65535)))
        table.add_row("Checksum", transport_data.get("checksum", "0x1234"))
        
        # Add flags
        flags = transport_data.get("flags", {})
        flags_str = ", ".join([flag for flag, value in flags.items() if value])
        table.add_row("Flags", flags_str if flags_str else "None")
        
        console.print(table)
        console.print()

class NetworkLayer(OSILayer):
    """Layer 3: Network Layer - IP"""
    
    def __init__(self):
        super().__init__("Network", 3)
    
    def encapsulate(self, data: Dict[str, Any]) -> Dict[str, Any]:
        """Create IP packet"""
        network_data = {
            "protocol": "IPv4",
            "source_ip": "192.168.1.100",
            "dest_ip": data.get("resolved_ip", "93.184.216.34"),
            "ttl": 64,
            "protocol_id": 6,  # TCP
            "header_checksum": "0x5678",
            "total_length": 1500,
            "identification": 12345,
            "flags": {
                "DF": True,
                "MF": False
            },
            "fragment_offset": 0
        }
        
        data["network"] = network_data
        return data
    
    def display(self, data: Dict[str, Any]) -> None:
        """Display IP packet details"""
        network_data = data.get("network", {})
        
        table = Table(title=f"🌐 Layer {self.layer_number}: {self.layer_name}", box=box.ROUNDED)
        table.add_column("Field", style="cyan", no_wrap=True)
        table.add_column("Value", style="green")
        
        table.add_row("Protocol", network_data.get("protocol", "IPv4"))
        table.add_row("Source IP", network_data.get("source_ip", "192.168.1.100"))
        table.add_row("Dest IP", network_data.get("dest_ip", "93.184.216.34"))
        table.add_row("TTL", str(network_data.get("ttl", 64)))
        table.add_row("Protocol ID", str(network_data.get("protocol_id", 6)))
        table.add_row("Total Length", str(network_data.get("total_length", 1500)))
        table.add_row("Identification", str(network_data.get("identification", 12345)))
        table.add_row("Header Checksum", network_data.get("header_checksum", "0x5678"))
        
        # Add flags
        flags = network_data.get("flags", {})
        flags_str = ", ".join([flag for flag, value in flags.items() if value])
        table.add_row("Flags", flags_str if flags_str else "None")
        table.add_row("Fragment Offset", str(network_data.get("fragment_offset", 0)))
        
        console.print(table)
        console.print()

class DataLinkLayer(OSILayer):
    """Layer 2: Data Link Layer - Ethernet"""
    
    def __init__(self):
        super().__init__("Data Link", 2)
    
    def encapsulate(self, data: Dict[str, Any]) -> Dict[str, Any]:
        """Create Ethernet frame"""
        datalink_data = {
            "protocol": "Ethernet II",
            "source_mac": "00:1B:44:11:3A:B7",
            "dest_mac": "00:50:56:C0:00:08",  # Gateway MAC
            "ethertype": "0x0800",  # IPv4
            "frame_size": 1518,
            "crc": "0xABCD1234"
        }
        
        data["datalink"] = datalink_data
        return data
    
    def display(self, data: Dict[str, Any]) -> None:
        """Display Ethernet frame details"""
        datalink_data = data.get("datalink", {})
        
        table = Table(title=f"🔗 Layer {self.layer_number}: {self.layer_name}", box=box.ROUNDED)
        table.add_column("Field", style="cyan", no_wrap=True)
        table.add_column("Value", style="green")
        
        table.add_row("Protocol", datalink_data.get("protocol", "Ethernet II"))
        table.add_row("Source MAC", datalink_data.get("source_mac", "00:1B:44:11:3A:B7"))
        table.add_row("Dest MAC", datalink_data.get("dest_mac", "00:50:56:C0:00:08"))
        table.add_row("EtherType", datalink_data.get("ethertype", "0x0800"))
        table.add_row("Frame Size", str(datalink_data.get("frame_size", 1518)))
        table.add_row("CRC", datalink_data.get("crc", "0xABCD1234"))
        
        console.print(table)
        console.print()

class PhysicalLayer(OSILayer):
    """Layer 1: Physical Layer - Bits transmission"""
    
    def __init__(self):
        super().__init__("Physical", 1)
    
    def encapsulate(self, data: Dict[str, Any]) -> Dict[str, Any]:
        """Add physical layer information"""
        physical_data = {
            "medium": "Ethernet Cable (Cat6)",
            "encoding": "Manchester",
            "data_rate": "1 Gbps",
            "signal_type": "Electrical",
            "bit_pattern": "10101010..."
        }
        
        data["physical"] = physical_data
        return data
    
    def display(self, data: Dict[str, Any]) -> None:
        """Display physical layer details"""
        physical_data = data.get("physical", {})
        
        table = Table(title=f"⚡ Layer {self.layer_number}: {self.layer_name}", box=box.ROUNDED)
        table.add_column("Field", style="cyan", no_wrap=True)
        table.add_column("Value", style="green")
        
        for key, value in physical_data.items():
            table.add_row(key.replace("_", " ").title(), str(value))
        
        console.print(table)
        console.print()

class PacketEncapsulator:
    """Main class to handle packet encapsulation through all OSI layers"""
    
    def __init__(self):
        self.layers = [
            ApplicationLayer(),
            PresentationLayer(),
            SessionLayer(),
            TransportLayer(),
            NetworkLayer(),
            DataLinkLayer(),
            PhysicalLayer()
        ]
    
    def encapsulate_packet(self, host: str, resolved_ip: str) -> Dict[str, Any]:
        """Encapsulate data through all OSI layers"""
        data = {
            "host": host,
            "resolved_ip": resolved_ip,
            "timestamp": time.time()
        }
        
        console.print(Panel.fit(
            f"🚀 [bold cyan]Starting Packet Encapsulation[/bold cyan]\n"
            f"Host: [bold green]{host}[/bold green] → IP: [bold yellow]{resolved_ip}[/bold yellow]",
            border_style="blue"
        ))
        console.print()
        
        # Process through layers (top-down: Application to Physical)
        for layer in self.layers:
            data = layer.encapsulate(data)
            layer.display(data)
            time.sleep(0.5)  # Visual delay
        
        return data
    
    def create_scapy_packet(self, data: Dict[str, Any]) -> Packet:
        """Create a Scapy packet from the encapsulated data"""
        try:
            # Extract data from our encapsulation
            network_data = data.get("network", {})
            transport_data = data.get("transport", {})
            
            # Create IP packet
            ip_pkt = IP(
                src=network_data.get("source_ip", "192.168.1.100"),
                dst=network_data.get("dest_ip", "93.184.216.34"),
                ttl=network_data.get("ttl", 64),
                id=network_data.get("identification", 12345)
            )
            
            # Create TCP segment
            tcp_pkt = TCP(
                sport=transport_data.get("source_port", 49152),
                dport=transport_data.get("dest_port", 80),
                seq=transport_data.get("sequence_number", 1000),
                flags="S"  # SYN flag
            )
            
            # Create HTTP payload
            app_data = data.get("application", {})
            http_payload = f"{app_data.get('method', 'GET')} {app_data.get('path', '/')} {app_data.get('version', 'HTTP/1.1')}\r\n"
            
            headers = app_data.get("headers", {})
            for key, value in headers.items():
                http_payload += f"{key}: {value}\r\n"
            http_payload += "\r\n"
            
            # Combine all layers
            packet = ip_pkt / tcp_pkt / Raw(load=http_payload)
            
            return packet
            
        except Exception as e:
            console.print(f"[red]Error creating Scapy packet: {e}[/red]")
            return None
    
    def display_packet_hex(self, packet: Packet) -> None:
        """Display packet in hexadecimal format"""
        if packet is None:
            return
        
        console.print(Panel.fit(
            "[bold magenta]📦 Packet Hex Dump[/bold magenta]",
            border_style="magenta"
        ))
        
        hex_dump = packet.show(dump=True)
        console.print(f"[dim]{hex_dump}[/dim]")
        console.print() 