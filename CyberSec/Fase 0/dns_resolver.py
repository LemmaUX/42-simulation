"""
DNS Resolution Module for Packet Odyssey
Handles domain name resolution with visual feedback
"""

from rich.console import Console
from rich.panel import Panel
from rich.table import Table
from rich.progress import Progress, SpinnerColumn, TextColumn
from rich import box
import socket
import time
from typing import Optional, Tuple

console = Console()

class DNSResolver:
    """Handles DNS resolution with visual feedback"""
    
    def __init__(self):
        self.dns_servers = [
            "8.8.8.8",      # Google DNS
            "1.1.1.1",      # Cloudflare DNS
            "208.67.222.222" # OpenDNS
        ]
    
    def resolve_domain(self, domain: str) -> Tuple[bool, Optional[str], Optional[str]]:
        """
        Resolve domain name to IP address with visual feedback
        
        Returns:
            Tuple of (success, ip_address, error_message)
        """
        console.print(Panel.fit(
            f"🔍 [bold cyan]DNS Resolution[/bold cyan]\n"
            f"Domain: [bold green]{domain}[/bold green]",
            border_style="cyan"
        ))
        console.print()
        
        # Show DNS servers
        table = Table(title="🌐 DNS Servers", box=box.ROUNDED)
        table.add_column("Server", style="cyan", no_wrap=True)
        table.add_column("Provider", style="green")
        
        for i, server in enumerate(self.dns_servers, 1):
            provider = "Google" if server == "8.8.8.8" else "Cloudflare" if server == "1.1.1.1" else "OpenDNS"
            table.add_row(f"{i}. {server}", provider)
        
        console.print(table)
        console.print()
        
        # Simulate DNS resolution process
        with Progress(
            SpinnerColumn(),
            TextColumn("[progress.description]{task.description}"),
            console=console
        ) as progress:
            
            # Step 1: Check local cache
            task1 = progress.add_task("Checking local DNS cache...", total=None)
            time.sleep(1)
            progress.update(task1, description="Local cache miss, querying DNS servers...")
            
            # Step 2: Query DNS servers
            task2 = progress.add_task("Querying DNS servers...", total=None)
            time.sleep(1.5)
            
            # Step 3: Resolve domain
            progress.update(task2, description="Resolving domain name...")
            time.sleep(1)
            
            try:
                # Actual DNS resolution
                ip_address = socket.gethostbyname(domain)
                progress.update(task2, description=f"✅ Resolved: {domain} → {ip_address}")
                time.sleep(0.5)
                
                # Display results
                self._display_resolution_results(domain, ip_address)
                return True, ip_address, None
                
            except socket.gaierror as e:
                error_msg = f"Failed to resolve {domain}: {str(e)}"
                progress.update(task2, description=f"❌ {error_msg}")
                time.sleep(0.5)
                return False, None, error_msg
            except Exception as e:
                error_msg = f"Unexpected error: {str(e)}"
                progress.update(task2, description=f"❌ {error_msg}")
                time.sleep(0.5)
                return False, None, error_msg
    
    def _display_resolution_results(self, domain: str, ip_address: str) -> None:
        """Display DNS resolution results"""
        table = Table(title="✅ DNS Resolution Results", box=box.ROUNDED)
        table.add_column("Field", style="cyan", no_wrap=True)
        table.add_column("Value", style="green")
        
        table.add_row("Domain", domain)
        table.add_row("IP Address", ip_address)
        table.add_row("Record Type", "A")
        table.add_row("TTL", "300 seconds")
        table.add_row("DNS Server", "8.8.8.8 (Google)")
        
        console.print(table)
        console.print()
        
        # Show reverse DNS lookup
        try:
            reverse_name = socket.gethostbyaddr(ip_address)[0]
            console.print(f"[dim]Reverse DNS: {ip_address} → {reverse_name}[/dim]")
        except:
            console.print(f"[dim]Reverse DNS: {ip_address} → No PTR record[/dim]")
        
        console.print()
    
    def simulate_dns_query(self, domain: str) -> str:
        """Simulate DNS query for offline mode"""
        console.print(Panel.fit(
            f"🔍 [bold cyan]DNS Resolution (Simulation)[/bold cyan]\n"
            f"Domain: [bold green]{domain}[/bold green]",
            border_style="cyan"
        ))
        console.print()
        
        # Simulate common domain resolutions
        common_domains = {
            "example.com": "93.184.216.34",
            "google.com": "142.250.185.78",
            "github.com": "140.82.113.4",
            "stackoverflow.com": "151.101.193.69",
            "reddit.com": "151.101.193.140"
        }
        
        if domain in common_domains:
            ip_address = common_domains[domain]
        else:
            # Generate a fake IP for unknown domains
            import random
            ip_address = f"192.168.{random.randint(1, 254)}.{random.randint(1, 254)}"
        
        time.sleep(1)
        console.print(f"[green]✅ Simulated resolution: {domain} → {ip_address}[/green]")
        console.print()
        
        return ip_address 