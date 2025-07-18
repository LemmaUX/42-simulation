#!/usr/bin/env python3
"""
Packet Odyssey Demo Script
Quick demonstration of the application's capabilities
"""

import sys
import os
import time

# Add current directory to path for imports
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

def run_demo():
    """Run a quick demo of Packet Odyssey"""
    print("🚀 Packet Odyssey - Quick Demo")
    print("=" * 50)
    print()
    
    try:
        from rich.console import Console
        from rich.panel import Panel
        from rich.table import Table
        from rich import box
        
        console = Console()
        
        # Demo banner
        console.print(Panel.fit(
            "[bold cyan]Packet Odyssey Demo[/bold cyan]\n"
            "Network Packet Journey Simulator\n"
            "Perfect for Cybersecurity Beginners",
            border_style="blue"
        ))
        console.print()
        
        # Feature overview
        features = Table(title="✨ Key Features", box=box.ROUNDED)
        features.add_column("Feature", style="cyan")
        features.add_column("Description", style="green")
        
        features.add_row("🔍 DNS Resolution", "Real-time domain name resolution")
        features.add_row("📦 OSI Encapsulation", "7-layer packet visualization")
        features.add_row("🗺️ Traceroute", "Network path analysis")
        features.add_row("📊 Packet Capture", "Real traffic analysis")
        features.add_row("🎨 Rich Interface", "Beautiful CLI with colors")
        features.add_row("🔄 Simulation Mode", "Offline learning capability")
        
        console.print(features)
        console.print()
        
        # Quick demo of DNS simulation
        console.print("[bold]🔍 DNS Resolution Demo:[/bold]")
        from dns_resolver import DNSResolver
        resolver = DNSResolver()
        ip = resolver.simulate_dns_query("example.com")
        console.print(f"✅ example.com → {ip}")
        console.print()
        
        # Quick demo of encapsulation
        console.print("[bold]📦 OSI Layer Demo:[/bold]")
        from layers import PacketEncapsulator
        encapsulator = PacketEncapsulator()
        data = encapsulator.encapsulate_packet("example.com", ip)
        console.print("✅ 7-layer encapsulation completed")
        console.print()
        
        # Usage examples
        console.print("[bold]🚀 Usage Examples:[/bold]")
        examples = Table(box=box.ROUNDED)
        examples.add_column("Command", style="cyan")
        examples.add_column("Description", style="green")
        
        examples.add_row("python main.py", "Interactive mode")
        examples.add_row("python main.py --full-journey", "Complete simulation")
        examples.add_row("python main.py --simulate", "Offline mode")
        examples.add_row("python main.py --dns google.com", "DNS only")
        examples.add_row("python main.py --traceroute github.com", "Traceroute only")
        
        console.print(examples)
        console.print()
        
        # Educational value
        console.print("[bold]🎓 Educational Value:[/bold]")
        console.print("• [cyan]OSI Model Understanding:[/cyan] Visualize how data flows through network layers")
        console.print("• [cyan]Network Security:[/cyan] Learn about vulnerabilities at each layer")
        console.print("• [cyan]Packet Analysis:[/cyan] Understand packet structure and headers")
        console.print("• [cyan]Network Troubleshooting:[/cyan] Practice with traceroute and DNS")
        console.print("• [cyan]Red/Blue Team Prep:[/cyan] Essential networking knowledge")
        console.print()
        
        console.print("[green]🎉 Demo completed! Ready to explore Packet Odyssey?[/green]")
        console.print()
        console.print("Run: [bold]python main.py[/bold] to start your journey!")
        
    except ImportError as e:
        print(f"❌ Import error: {e}")
        print("Please install dependencies: pip install -r requirements.txt")
    except Exception as e:
        print(f"❌ Demo error: {e}")

if __name__ == "__main__":
    run_demo() 