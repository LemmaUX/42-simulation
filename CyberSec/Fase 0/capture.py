"""
Packet Capture Module for Packet Odyssey
Handles real or simulated packet capture with analysis
"""

import os
import subprocess
import platform
import time
import random
from typing import List, Dict, Optional, Tuple
from rich.console import Console
from rich.panel import Panel
from rich.table import Table
from rich.progress import Progress, SpinnerColumn, TextColumn
from rich import box
from scapy.all import *

console = Console()

class PacketCapture:
    """Handles packet capture functionality with cross-platform support"""
    
    def __init__(self):
        self.system = platform.system().lower()
        self.capture_duration = 10  # seconds
        self.max_packets = 50
        self.pcap_file = "packet_odyssey_capture.pcap"
    
    def capture_packets(self, target: str, simulate: bool = False) -> List[Dict[str, any]]:
        """
        Capture packets to/from target
        
        Args:
            target: Target IP or domain
            simulate: Whether to simulate packet capture
            
        Returns:
            List of captured packet information
        """
        console.print(Panel.fit(
            f"📦 [bold cyan]Packet Capture[/bold cyan]\n"
            f"Target: [bold green]{target}[/bold green]\n"
            f"Duration: [bold yellow]{self.capture_duration}s[/bold yellow]",
            border_style="cyan"
        ))
        console.print()
        
        if simulate:
            return self._simulate_capture(target)
        else:
            return self._real_capture(target)
    
    def _real_capture(self, target: str) -> List[Dict[str, any]]:
        """Perform real packet capture using tcpdump or scapy"""
        packets = []
        
        try:
            # Check if we have permission to capture
            if not self._check_capture_permissions():
                console.print("[yellow]⚠️ Insufficient permissions for packet capture, using simulation[/yellow]")
                return self._simulate_capture(target)
            
            console.print("[green]🔍 Starting real packet capture...[/green]")
            console.print()
            
            # Use scapy for packet capture
            with Progress(
                SpinnerColumn(),
                TextColumn("[progress.description]{task.description}"),
                console=console
            ) as progress:
                task = progress.add_task("Capturing packets...", total=self.capture_duration)
                
                # Capture packets using scapy
                captured_packets = sniff(
                    filter=f"host {target}",
                    timeout=self.capture_duration,
                    count=self.max_packets,
                    prn=lambda pkt: self._process_packet(pkt, packets)
                )
                
                for i in range(self.capture_duration):
                    progress.update(task, completed=i+1)
                    time.sleep(1)
            
            # Save to pcap file
            if packets:
                wrpcap(self.pcap_file, captured_packets)
                console.print(f"[green]✅ Captured {len(packets)} packets, saved to {self.pcap_file}[/green]")
            
        except Exception as e:
            console.print(f"[red]❌ Error during packet capture: {e}[/red]")
            console.print("[yellow]🔄 Falling back to simulation mode[/yellow]")
            return self._simulate_capture(target)
        
        return packets
    
    def _simulate_capture(self, target: str) -> List[Dict[str, any]]:
        """Simulate packet capture for offline mode or when real capture fails"""
        console.print("[yellow]🔄 Running packet capture simulation[/yellow]")
        console.print()
        
        packets = []
        
        with Progress(
            SpinnerColumn(),
            TextColumn("[progress.description]{task.description}"),
            console=console
        ) as progress:
            task = progress.add_task("Simulating packet capture...", total=self.capture_duration)
            
            for i in range(self.capture_duration):
                # Generate simulated packets
                num_packets = random.randint(1, 5)
                for _ in range(num_packets):
                    packet_info = self._generate_simulated_packet(target, i)
                    packets.append(packet_info)
                
                progress.update(task, completed=i+1)
                time.sleep(1)
            
            progress.update(task, description="✅ Capture simulation complete")
        
        # Save simulated capture
        self._save_simulated_pcap(packets)
        
        return packets
    
    def _check_capture_permissions(self) -> bool:
        """Check if we have permission to capture packets"""
        try:
            # Try to create a simple capture to test permissions
            test_capture = sniff(timeout=1, count=1)
            return True
        except:
            return False
    
    def _process_packet(self, packet: Packet, packets_list: List[Dict[str, any]]) -> None:
        """Process a captured packet and extract relevant information"""
        try:
            packet_info = {
                "timestamp": time.time(),
                "length": len(packet),
                "protocol": "Unknown",
                "source": "Unknown",
                "destination": "Unknown",
                "info": str(packet.summary())
            }
            
            # Extract IP layer info
            if IP in packet:
                packet_info["source"] = packet[IP].src
                packet_info["destination"] = packet[IP].dst
                packet_info["protocol"] = "IP"
                
                # Extract transport layer info
                if TCP in packet:
                    packet_info["protocol"] = "TCP"
                    packet_info["source_port"] = packet[TCP].sport
                    packet_info["dest_port"] = packet[TCP].dport
                    packet_info["flags"] = str(packet[TCP].flags)
                elif UDP in packet:
                    packet_info["protocol"] = "UDP"
                    packet_info["source_port"] = packet[UDP].sport
                    packet_info["dest_port"] = packet[UDP].dport
                elif ICMP in packet:
                    packet_info["protocol"] = "ICMP"
                    packet_info["type"] = packet[ICMP].type
            
            packets_list.append(packet_info)
            
        except Exception as e:
            console.print(f"[red]Error processing packet: {e}[/red]")
    
    def _generate_simulated_packet(self, target: str, timestamp: int) -> Dict[str, any]:
        """Generate a simulated packet for offline mode"""
        protocols = ["TCP", "UDP", "ICMP", "HTTP", "HTTPS"]
        protocol = random.choice(protocols)
        
        # Generate realistic packet sizes
        if protocol in ["HTTP", "HTTPS"]:
            length = random.randint(500, 1500)
        elif protocol == "TCP":
            length = random.randint(60, 1500)
        elif protocol == "UDP":
            length = random.randint(28, 1500)
        else:  # ICMP
            length = random.randint(28, 100)
        
        packet_info = {
            "timestamp": time.time() + timestamp,
            "length": length,
            "protocol": protocol,
            "source": f"192.168.{random.randint(1, 254)}.{random.randint(1, 254)}",
            "destination": target,
            "info": f"Simulated {protocol} packet"
        }
        
        # Add protocol-specific info
        if protocol == "TCP":
            packet_info["source_port"] = random.randint(49152, 65535)
            packet_info["dest_port"] = random.choice([80, 443, 22, 53])
            packet_info["flags"] = random.choice(["S", "A", "PA", "FA"])
        elif protocol == "UDP":
            packet_info["source_port"] = random.randint(49152, 65535)
            packet_info["dest_port"] = random.choice([53, 67, 123, 161])
        elif protocol == "ICMP":
            packet_info["type"] = random.choice([0, 3, 8, 11])
        
        return packet_info
    
    def _save_simulated_pcap(self, packets: List[Dict[str, any]]) -> None:
        """Save simulated packets to a pcap file"""
        try:
            # Create a simple pcap file with simulated packets
            pkt_list = []
            
            for packet_info in packets:
                # Create basic IP packet
                if packet_info["protocol"] == "TCP":
                    pkt = IP(src=packet_info["source"], dst=packet_info["destination"]) / \
                          TCP(sport=packet_info.get("source_port", 12345), 
                              dport=packet_info.get("dest_port", 80))
                elif packet_info["protocol"] == "UDP":
                    pkt = IP(src=packet_info["source"], dst=packet_info["destination"]) / \
                          UDP(sport=packet_info.get("source_port", 12345), 
                              dport=packet_info.get("dest_port", 53))
                else:
                    pkt = IP(src=packet_info["source"], dst=packet_info["destination"]) / \
                          ICMP(type=packet_info.get("type", 8))
                
                pkt_list.append(pkt)
            
            if pkt_list:
                wrpcap(self.pcap_file, pkt_list)
                console.print(f"[green]✅ Simulated capture saved to {self.pcap_file}[/green]")
        
        except Exception as e:
            console.print(f"[red]❌ Error saving pcap file: {e}[/red]")
    
    def display_capture_results(self, packets: List[Dict[str, any]]) -> None:
        """Display captured packets in a formatted table"""
        if not packets:
            console.print("[red]❌ No packets captured[/red]")
            return
        
        console.print(Panel.fit(
            f"[bold magenta]📊 Packet Capture Summary[/bold magenta]\n"
            f"Total Packets: [bold green]{len(packets)}[/bold green]",
            border_style="magenta"
        ))
        console.print()
        
        # Protocol distribution
        protocol_counts = {}
        for packet in packets:
            protocol = packet.get("protocol", "Unknown")
            protocol_counts[protocol] = protocol_counts.get(protocol, 0) + 1
        
        table = Table(title="📈 Protocol Distribution", box=box.ROUNDED)
        table.add_column("Protocol", style="cyan")
        table.add_column("Count", style="green", justify="right")
        table.add_column("Percentage", style="yellow", justify="right")
        
        total_packets = len(packets)
        for protocol, count in sorted(protocol_counts.items(), key=lambda x: x[1], reverse=True):
            percentage = (count / total_packets) * 100
            table.add_row(protocol, str(count), f"{percentage:.1f}%")
        
        console.print(table)
        console.print()
        
        # Recent packets table
        recent_packets = packets[-10:]  # Show last 10 packets
        
        table = Table(title="📦 Recent Packets", box=box.ROUNDED)
        table.add_column("Time", style="cyan")
        table.add_column("Protocol", style="green")
        table.add_column("Source", style="blue")
        table.add_column("Destination", style="blue")
        table.add_column("Length", style="yellow", justify="right")
        table.add_column("Info", style="white")
        
        for packet in recent_packets:
            timestamp = time.strftime("%H:%M:%S", time.localtime(packet["timestamp"]))
            table.add_row(
                timestamp,
                packet.get("protocol", "Unknown"),
                packet.get("source", "Unknown"),
                packet.get("destination", "Unknown"),
                str(packet.get("length", 0)),
                packet.get("info", "")[:30] + "..." if len(packet.get("info", "")) > 30 else packet.get("info", "")
            )
        
        console.print(table)
        console.print()
        
        # Statistics
        total_bytes = sum(p.get("length", 0) for p in packets)
        avg_packet_size = total_bytes / len(packets) if packets else 0
        
        console.print(f"[dim]Statistics: Total bytes: {total_bytes}, "
                     f"Average packet size: {avg_packet_size:.1f} bytes[/dim]")
        console.print()
    
    def analyze_captured_traffic(self, packets: List[Dict[str, any]]) -> None:
        """Analyze captured traffic for patterns and anomalies"""
        if not packets:
            return
        
        console.print(Panel.fit(
            "[bold magenta]🔍 Traffic Analysis[/bold magenta]",
            border_style="magenta"
        ))
        
        # Analyze traffic patterns
        analysis = {
            "total_packets": len(packets),
            "total_bytes": sum(p.get("length", 0) for p in packets),
            "protocols": {},
            "ports": {},
            "suspicious_activity": []
        }
        
        # Count protocols and ports
        for packet in packets:
            protocol = packet.get("protocol", "Unknown")
            analysis["protocols"][protocol] = analysis["protocols"].get(protocol, 0) + 1
            
            if "source_port" in packet:
                port = packet["source_port"]
                analysis["ports"][port] = analysis["ports"].get(port, 0) + 1
        
        # Detect suspicious activity
        for packet in packets:
            # Check for unusual ports
            if packet.get("dest_port") in [22, 23, 3389, 1433, 3306]:
                analysis["suspicious_activity"].append(f"Access to admin port {packet.get('dest_port')}")
            
            # Check for large packets
            if packet.get("length", 0) > 1400:
                analysis["suspicious_activity"].append(f"Large packet detected ({packet.get('length')} bytes)")
        
        # Display analysis
        table = Table(title="📊 Traffic Analysis", box=box.ROUNDED)
        table.add_column("Metric", style="cyan")
        table.add_column("Value", style="green")
        
        table.add_row("Total Packets", str(analysis["total_packets"]))
        table.add_row("Total Bytes", str(analysis["total_bytes"]))
        table.add_row("Unique Protocols", str(len(analysis["protocols"])))
        table.add_row("Unique Ports", str(len(analysis["ports"])))
        table.add_row("Avg Packet Size", f"{analysis['total_bytes'] / analysis['total_packets']:.1f} bytes")
        
        console.print(table)
        console.print()
        
        # Display suspicious activity
        if analysis["suspicious_activity"]:
            console.print("[red]🚨 Suspicious Activity Detected:[/red]")
            for activity in set(analysis["suspicious_activity"]):
                console.print(f"  • {activity}")
            console.print()
        else:
            console.print("[green]✅ No suspicious activity detected[/green]")
            console.print()
        
        # Recommendations
        console.print("[blue]💡 Recommendations:[/blue]")
        if len(analysis["protocols"]) > 5:
            console.print("  • High protocol diversity - consider traffic filtering")
        
        if analysis["total_packets"] > 100:
            console.print("  • High packet volume - monitor for DDoS indicators")
        
        if not analysis["suspicious_activity"]:
            console.print("  • Traffic appears normal - continue monitoring")
        
        console.print() 