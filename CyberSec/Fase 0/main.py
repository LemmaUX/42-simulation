#!/usr/bin/env python3
"""
Packet Odyssey - Network Packet Journey Simulator
A beginner-friendly tool to visualize network packet journeys across OSI layers

Author: Packet Odyssey Team
Version: 1.0.0
"""

import argparse
import sys
import os
from rich.console import Console
from rich.panel import Panel
from rich.table import Table
from rich import box
from rich.prompt import Prompt, Confirm
import time

# Import our modules
from layers import PacketEncapsulator
from dns_resolver import DNSResolver
from tracer import Traceroute
from capture import PacketCapture

console = Console()

class PacketOdyssey:
    """Main application class for Packet Odyssey"""
    
    def __init__(self):
        self.encapsulator = PacketEncapsulator()
        self.dns_resolver = DNSResolver()
        self.traceroute = Traceroute()
        self.capture = PacketCapture()
        
    def display_banner(self):
        """Display the application banner"""
        banner = """
╔══════════════════════════════════════════════════════════════╗
║                    🚀 PACKET ODYSSEY 🚀                      ║
║                                                              ║
║         Network Packet Journey Simulator v1.0.0              ║
║         Exploring the OSI Model Layer by Layer               ║
║                                                              ║
║         Perfect for Cybersecurity Beginners                  ║
║         Red/Blue Team Preparation Tool                       ║
╚══════════════════════════════════════════════════════════════╝
        """
        console.print(Panel.fit(banner, border_style="blue"))
        console.print()
    
    def display_menu(self):
        """Display the main menu"""
        menu = Table(title="🎯 Packet Odyssey Menu", box=box.ROUNDED)
        menu.add_column("Option", style="cyan", no_wrap=True)
        menu.add_column("Description", style="green")
        
        menu.add_row("1", "🔍 DNS Resolution")
        menu.add_row("2", "📦 Packet Encapsulation (OSI Layers)")
        menu.add_row("3", "🗺️ Traceroute Analysis")
        menu.add_row("4", "📊 Packet Capture")
        menu.add_row("5", "🚀 Full Journey Simulation")
        menu.add_row("6", "📚 OSI Model Guide")
        menu.add_row("7", "⚙️ Settings")
        menu.add_row("0", "🚪 Exit")
        
        console.print(menu)
        console.print()
    
    def run_dns_resolution(self, simulate: bool = False):
        """Run DNS resolution module"""
        console.print(Panel.fit(
            "[bold cyan]🔍 DNS Resolution Module[/bold cyan]",
            border_style="cyan"
        ))
        console.print()
        
        # Get target domain
        if simulate:
            domain = "example.com"
            console.print(f"[dim]Simulation mode: Using {domain}[/dim]")
        else:
            domain = Prompt.ask("Enter domain name", default="example.com")
        
        console.print()
        
        # Perform DNS resolution
        if simulate:
            ip_address = self.dns_resolver.simulate_dns_query(domain)
            success = True
        else:
            success, ip_address, error = self.dns_resolver.resolve_domain(domain)
        
        if success and ip_address:
            console.print(f"[green]✅ Successfully resolved {domain} to {ip_address}[/green]")
            return ip_address
        else:
            console.print(f"[red]❌ Failed to resolve {domain}[/red]")
            if not simulate:
                console.print(f"Error: {error}")
            return None
    
    def run_packet_encapsulation(self, host: str, ip_address: str):
        """Run packet encapsulation module"""
        console.print(Panel.fit(
            "[bold cyan]📦 Packet Encapsulation Module[/bold cyan]",
            border_style="cyan"
        ))
        console.print()
        
        # Perform packet encapsulation
        encapsulated_data = self.encapsulator.encapsulate_packet(host, ip_address)
        
        # Create and display Scapy packet
        packet = self.encapsulator.create_scapy_packet(encapsulated_data)
        if packet:
            self.encapsulator.display_packet_hex(packet)
        
        return encapsulated_data
    
    def run_traceroute(self, target: str, simulate: bool = False):
        """Run traceroute module"""
        console.print(Panel.fit(
            "[bold cyan]🗺️ Traceroute Analysis Module[/bold cyan]",
            border_style="cyan"
        ))
        console.print()
        
        # Perform traceroute
        hops = self.traceroute.trace_route(target, simulate)
        
        # Display results
        self.traceroute.display_traceroute_results(hops)
        
        # Analyze network path
        self.traceroute.analyze_network_path(hops)
        
        return hops
    
    def run_packet_capture(self, target: str, simulate: bool = False):
        """Run packet capture module"""
        console.print(Panel.fit(
            "[bold cyan]📊 Packet Capture Module[/bold cyan]",
            border_style="cyan"
        ))
        console.print()
        
        # Perform packet capture
        packets = self.capture.capture_packets(target, simulate)
        
        # Display results
        self.capture.display_capture_results(packets)
        
        # Analyze traffic
        self.capture.analyze_captured_traffic(packets)
        
        return packets
    
    def run_full_journey(self, simulate: bool = False):
        """Run the complete packet journey simulation"""
        console.print(Panel.fit(
            "[bold cyan]🚀 Full Packet Journey Simulation[/bold cyan]",
            border_style="cyan"
        ))
        console.print()
        
        # Get target
        if simulate:
            host = "example.com"
            console.print(f"[dim]Simulation mode: Using {host}[/dim]")
        else:
            host = Prompt.ask("Enter target domain", default="example.com")
        
        console.print()
        
        # Step 1: DNS Resolution
        console.print("=" * 60)
        console.print("[bold]Step 1: DNS Resolution[/bold]")
        console.print("=" * 60)
        ip_address = self.run_dns_resolution(simulate)
        
        if not ip_address:
            console.print("[red]❌ Cannot proceed without IP address[/red]")
            return
        
        console.print()
        
        # Step 2: Packet Encapsulation
        console.print("=" * 60)
        console.print("[bold]Step 2: Packet Encapsulation (OSI Layers)[/bold]")
        console.print("=" * 60)
        encapsulated_data = self.run_packet_encapsulation(host, ip_address)
        
        console.print()
        
        # Step 3: Traceroute
        console.print("=" * 60)
        console.print("[bold]Step 3: Network Path Analysis[/bold]")
        console.print("=" * 60)
        hops = self.run_traceroute(ip_address, simulate)
        
        console.print()
        
        # Step 4: Packet Capture
        console.print("=" * 60)
        console.print("[bold]Step 4: Packet Capture & Analysis[/bold]")
        console.print("=" * 60)
        packets = self.run_packet_capture(ip_address, simulate)
        
        console.print()
        
        # Summary
        console.print("=" * 60)
        console.print("[bold]🎉 Journey Complete![/bold]")
        console.print("=" * 60)
        
        summary = Table(title="📋 Journey Summary", box=box.ROUNDED)
        summary.add_column("Component", style="cyan")
        summary.add_column("Status", style="green")
        summary.add_column("Details", style="yellow")
        
        summary.add_row("DNS Resolution", "✅ Complete", f"Resolved {host} to {ip_address}")
        summary.add_row("OSI Encapsulation", "✅ Complete", "7 layers processed")
        summary.add_row("Traceroute", "✅ Complete", f"{len(hops)} hops analyzed")
        summary.add_row("Packet Capture", "✅ Complete", f"{len(packets)} packets captured")
        
        console.print(summary)
        console.print()
        
        console.print("[green]🎓 You've successfully completed the Packet Odyssey![/green]")
        console.print("[dim]This simulation demonstrates how network packets travel through the OSI model layers.[/dim]")
        console.print()
    
    def display_osi_guide(self):
        """Display OSI model guide"""
        console.print(Panel.fit(
            "[bold cyan]📚 OSI Model Guide[/bold cyan]",
            border_style="cyan"
        ))
        console.print()
        
        guide = Table(title="🌐 The 7 OSI Layers", box=box.ROUNDED)
        guide.add_column("Layer", style="cyan", justify="center")
        guide.add_column("Name", style="green")
        guide.add_column("Function", style="yellow")
        guide.add_column("Protocols", style="blue")
        
        guide.add_row("7", "Application", "User interface and services", "HTTP, FTP, SMTP, DNS")
        guide.add_row("6", "Presentation", "Data formatting and encryption", "SSL/TLS, JPEG, MPEG")
        guide.add_row("5", "Session", "Session management", "NetBIOS, RPC, SQL")
        guide.add_row("4", "Transport", "End-to-end communication", "TCP, UDP")
        guide.add_row("3", "Network", "Logical addressing and routing", "IP, ICMP, OSPF")
        guide.add_row("2", "Data Link", "Physical addressing", "Ethernet, WiFi, PPP")
        guide.add_row("1", "Physical", "Physical transmission", "Cables, signals, bits")
        
        console.print(guide)
        console.print()
        
        console.print("[bold]💡 Key Concepts:[/bold]")
        console.print("• [cyan]Encapsulation:[/cyan] Each layer adds its own header to the data")
        console.print("• [cyan]De-encapsulation:[/cyan] Headers are removed as data moves up the stack")
        console.print("• [cyan]Protocol Data Units (PDUs):[/cyan] Different names for data at each layer")
        console.print("• [cyan]Peer-to-Peer Communication:[/cyan] Each layer communicates with its peer")
        console.print()
        
        console.print("[bold]🔍 Cybersecurity Relevance:[/bold]")
        console.print("• [red]Layer 7:[/red] Application attacks (SQL injection, XSS)")
        console.print("• [red]Layer 4:[/red] Transport attacks (SYN floods, port scanning)")
        console.print("• [red]Layer 3:[/red] Network attacks (IP spoofing, routing attacks)")
        console.print("• [red]Layer 2:[/red] Data link attacks (ARP spoofing, MAC flooding)")
        console.print()
    
    def display_settings(self):
        """Display and modify settings"""
        console.print(Panel.fit(
            "[bold cyan]⚙️ Settings[/bold cyan]",
            border_style="cyan"
        ))
        console.print()
        
        settings = Table(title="Current Settings", box=box.ROUNDED)
        settings.add_column("Setting", style="cyan")
        settings.add_column("Value", style="green")
        settings.add_column("Description", style="yellow")
        
        settings.add_row("Capture Duration", f"{self.capture.capture_duration}s", "Packet capture time")
        settings.add_row("Max Packets", str(self.capture.max_packets), "Maximum packets to capture")
        settings.add_row("Max Hops", str(self.traceroute.max_hops), "Maximum traceroute hops")
        settings.add_row("Timeout", f"{self.traceroute.timeout}s", "Traceroute timeout")
        
        console.print(settings)
        console.print()
        
        # Allow user to modify settings
        if Confirm.ask("Would you like to modify settings?"):
            try:
                new_duration = int(Prompt.ask("Capture duration (seconds)", default=str(self.capture.capture_duration)))
                self.capture.capture_duration = new_duration
                
                new_max_packets = int(Prompt.ask("Max packets", default=str(self.capture.max_packets)))
                self.capture.max_packets = new_max_packets
                
                new_max_hops = int(Prompt.ask("Max hops", default=str(self.traceroute.max_hops)))
                self.traceroute.max_hops = new_max_hops
                
                new_timeout = int(Prompt.ask("Timeout (seconds)", default=str(self.traceroute.timeout)))
                self.traceroute.timeout = new_timeout
                
                console.print("[green]✅ Settings updated successfully![/green]")
                
            except ValueError:
                console.print("[red]❌ Invalid input. Settings unchanged.[/red]")
        
        console.print()
    
    def run_interactive_mode(self):
        """Run the application in interactive mode"""
        while True:
            self.display_banner()
            self.display_menu()
            
            choice = Prompt.ask("Select an option", choices=["0", "1", "2", "3", "4", "5", "6", "7"])
            
            if choice == "0":
                console.print("[green]👋 Thank you for using Packet Odyssey![/green]")
                break
            elif choice == "1":
                self.run_dns_resolution()
            elif choice == "2":
                host = Prompt.ask("Enter target domain", default="example.com")
                ip_address = self.run_dns_resolution()
                if ip_address:
                    self.run_packet_encapsulation(host, ip_address)
            elif choice == "3":
                target = Prompt.ask("Enter target", default="example.com")
                self.run_traceroute(target)
            elif choice == "4":
                target = Prompt.ask("Enter target", default="example.com")
                self.run_packet_capture(target)
            elif choice == "5":
                self.run_full_journey()
            elif choice == "6":
                self.display_osi_guide()
            elif choice == "7":
                self.display_settings()
            
            if choice != "0":
                console.print()
                if Confirm.ask("Press Enter to continue..."):
                    console.clear()
    
    def run_cli_mode(self, args):
        """Run the application in CLI mode based on arguments"""
        self.display_banner()
        
        if args.simulate:
            console.print("[yellow]🔄 Running in simulation mode[/yellow]")
            console.print()
        
        if args.full_journey:
            self.run_full_journey(args.simulate)
        elif args.dns:
            self.run_dns_resolution(args.simulate)
        elif args.encapsulation:
            host = args.target or "example.com"
            ip_address = self.run_dns_resolution(args.simulate)
            if ip_address:
                self.run_packet_encapsulation(host, ip_address)
        elif args.traceroute:
            target = args.target or "example.com"
            self.run_traceroute(target, args.simulate)
        elif args.capture:
            target = args.target or "example.com"
            self.run_packet_capture(target, args.simulate)
        else:
            # Default to interactive mode
            self.run_interactive_mode()

def main():
    """Main entry point"""
    parser = argparse.ArgumentParser(
        description="Packet Odyssey - Network Packet Journey Simulator",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python main.py                    # Interactive mode
  python main.py --full-journey     # Complete simulation
  python main.py --dns example.com  # DNS resolution only
  python main.py --simulate         # Run in simulation mode
  python main.py --traceroute google.com  # Traceroute to Google
        """
    )
    
    parser.add_argument("target", nargs="?", help="Target domain or IP address")
    parser.add_argument("--simulate", action="store_true", help="Run in simulation mode")
    parser.add_argument("--full-journey", action="store_true", help="Run complete packet journey")
    parser.add_argument("--dns", action="store_true", help="DNS resolution only")
    parser.add_argument("--encapsulation", action="store_true", help="Packet encapsulation only")
    parser.add_argument("--traceroute", action="store_true", help="Traceroute only")
    parser.add_argument("--capture", action="store_true", help="Packet capture only")
    
    args = parser.parse_args()
    
    try:
        app = PacketOdyssey()
        app.run_cli_mode(args)
    except KeyboardInterrupt:
        console.print("\n[red]❌ Interrupted by user[/red]")
        sys.exit(1)
    except Exception as e:
        console.print(f"[red]❌ Unexpected error: {e}[/red]")
        sys.exit(1)

if __name__ == "__main__":
    main() 