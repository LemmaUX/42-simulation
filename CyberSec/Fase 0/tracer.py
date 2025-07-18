"""
Traceroute Module for Packet Odyssey
Handles network path discovery with hop-by-hop analysis
"""

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

console = Console()

class Traceroute:
    """Handles traceroute functionality with cross-platform support"""
    
    def __init__(self):
        self.system = platform.system().lower()
        self.max_hops = 30
        self.timeout = 3
    
    def trace_route(self, target: str, simulate: bool = False) -> List[Dict[str, any]]:
        """
        Perform traceroute to target
        
        Args:
            target: Target IP or domain
            simulate: Whether to simulate the traceroute
            
        Returns:
            List of hop information dictionaries
        """
        console.print(Panel.fit(
            f"🗺️ [bold cyan]Traceroute Analysis[/bold cyan]\n"
            f"Target: [bold green]{target}[/bold green]",
            border_style="cyan"
        ))
        console.print()
        
        if simulate:
            return self._simulate_traceroute(target)
        else:
            return self._real_traceroute(target)
    
    def _real_traceroute(self, target: str) -> List[Dict[str, any]]:
        """Perform real traceroute using system command"""
        hops = []
        
        try:
            # Determine traceroute command based on OS
            if self.system == "windows":
                cmd = ["tracert", "-h", str(self.max_hops), "-w", str(self.timeout * 1000), target]
            else:
                cmd = ["traceroute", "-m", str(self.max_hops), "-w", str(self.timeout), target]
            
            console.print(f"[dim]Executing: {' '.join(cmd)}[/dim]")
            console.print()
            
            # Execute traceroute
            with Progress(
                SpinnerColumn(),
                TextColumn("[progress.description]{task.description}"),
                console=console
            ) as progress:
                task = progress.add_task("Tracing route...", total=None)
                
                result = subprocess.run(
                    cmd,
                    capture_output=True,
                    text=True,
                    timeout=30
                )
                
                progress.update(task, description="Parsing results...")
                time.sleep(1)
            
            # Parse output
            hops = self._parse_traceroute_output(result.stdout, result.stderr)
            
        except subprocess.TimeoutExpired:
            console.print("[red]❌ Traceroute timed out[/red]")
            return self._simulate_traceroute(target)
        except FileNotFoundError:
            console.print("[yellow]⚠️ Traceroute command not found, using simulation[/yellow]")
            return self._simulate_traceroute(target)
        except Exception as e:
            console.print(f"[red]❌ Error during traceroute: {e}[/red]")
            return self._simulate_traceroute(target)
        
        return hops
    
    def _simulate_traceroute(self, target: str) -> List[Dict[str, any]]:
        """Simulate traceroute for offline mode or when real traceroute fails"""
        console.print("[yellow]🔄 Running in simulation mode[/yellow]")
        console.print()
        
        hops = []
        
        with Progress(
            SpinnerColumn(),
            TextColumn("[progress.description]{task.description}"),
            console=console
        ) as progress:
            
            # Simulate network path
            task = progress.add_task("Simulating network path...", total=None)
            
            # Generate realistic hop sequence
            hop_sequence = self._generate_hop_sequence(target)
            
            for i, hop_info in enumerate(hop_sequence):
                progress.update(task, description=f"Hop {i+1}: {hop_info['ip']}")
                time.sleep(0.3)
                
                hops.append(hop_info)
            
            progress.update(task, description="✅ Traceroute complete")
            time.sleep(0.5)
        
        return hops
    
    def _generate_hop_sequence(self, target: str) -> List[Dict[str, any]]:
        """Generate realistic hop sequence for simulation"""
        hops = []
        
        # Common network topology simulation
        network_path = [
            {"ip": "192.168.1.1", "hostname": "router.local", "latency": 1.2, "ttl": 64},
            {"ip": "10.0.0.1", "hostname": "gateway.isp.com", "latency": 5.8, "ttl": 63},
            {"ip": "172.16.0.1", "hostname": "core-router.isp.com", "latency": 12.3, "ttl": 62},
            {"ip": "203.0.113.1", "hostname": "border-router.isp.com", "latency": 18.7, "ttl": 61},
            {"ip": "198.51.100.1", "hostname": "peering-router.isp.com", "latency": 25.4, "ttl": 60},
            {"ip": "203.0.113.254", "hostname": "transit-router.net", "latency": 32.1, "ttl": 59},
            {"ip": "198.51.100.254", "hostname": "backbone-router.net", "latency": 45.6, "ttl": 58},
            {"ip": "192.0.2.1", "hostname": "destination-router.com", "latency": 52.3, "ttl": 57},
            {"ip": target, "hostname": target, "latency": 58.9, "ttl": 56}
        ]
        
        # Add some randomness and packet loss simulation
        for i, hop in enumerate(network_path):
            # Simulate occasional packet loss
            if random.random() < 0.1:  # 10% chance of packet loss
                hop["status"] = "*"
                hop["latency"] = None
            else:
                hop["status"] = "OK"
                # Add some latency variation
                hop["latency"] += random.uniform(-2, 2)
                hop["latency"] = max(0.1, hop["latency"])
            
            hop["hop_number"] = i + 1
            hops.append(hop)
        
        return hops
    
    def _parse_traceroute_output(self, stdout: str, stderr: str) -> List[Dict[str, any]]:
        """Parse traceroute command output"""
        hops = []
        lines = stdout.split('\n')
        
        for line in lines:
            line = line.strip()
            if not line or line.startswith('traceroute') or line.startswith('tracert'):
                continue
            
            # Parse hop information (simplified parsing)
            hop_info = self._parse_hop_line(line)
            if hop_info:
                hops.append(hop_info)
        
        return hops
    
    def _parse_hop_line(self, line: str) -> Optional[Dict[str, any]]:
        """Parse a single hop line from traceroute output"""
        try:
            # This is a simplified parser - real implementation would be more robust
            parts = line.split()
            if len(parts) < 2:
                return None
            
            hop_num = int(parts[0])
            ip = parts[1]
            
            # Extract latency if available
            latency = None
            for part in parts[2:]:
                if part.replace('.', '').replace('ms', '').isdigit():
                    latency = float(part.replace('ms', ''))
                    break
            
            return {
                "hop_number": hop_num,
                "ip": ip,
                "hostname": ip,  # Simplified
                "latency": latency,
                "status": "OK" if latency else "*",
                "ttl": 64 - hop_num
            }
        except:
            return None
    
    def display_traceroute_results(self, hops: List[Dict[str, any]]) -> None:
        """Display traceroute results in a formatted table"""
        if not hops:
            console.print("[red]❌ No traceroute data available[/red]")
            return
        
        table = Table(title="🗺️ Traceroute Results", box=box.ROUNDED)
        table.add_column("Hop", style="cyan", justify="center")
        table.add_column("IP Address", style="green")
        table.add_column("Hostname", style="blue")
        table.add_column("Latency (ms)", style="yellow", justify="right")
        table.add_column("TTL", style="magenta", justify="center")
        table.add_column("Status", style="red")
        
        for hop in hops:
            latency_str = f"{hop['latency']:.1f}" if hop['latency'] else "*"
            status = hop.get('status', 'OK')
            status_style = "green" if status == "OK" else "red"
            
            table.add_row(
                str(hop['hop_number']),
                hop['ip'],
                hop.get('hostname', ''),
                latency_str,
                str(hop.get('ttl', '')),
                f"[{status_style}]{status}[/{status_style}]"
            )
        
        console.print(table)
        console.print()
        
        # Summary statistics
        successful_hops = [h for h in hops if h.get('status') == 'OK' and h.get('latency')]
        if successful_hops:
            avg_latency = sum(h['latency'] for h in successful_hops) / len(successful_hops)
            total_hops = len(hops)
            successful_count = len(successful_hops)
            
            console.print(f"[dim]Summary: {successful_count}/{total_hops} hops successful, "
                         f"average latency: {avg_latency:.1f}ms[/dim]")
            console.print()
    
    def analyze_network_path(self, hops: List[Dict[str, any]]) -> None:
        """Analyze the network path and provide insights"""
        if not hops:
            return
        
        console.print(Panel.fit(
            "[bold magenta]🔍 Network Path Analysis[/bold magenta]",
            border_style="magenta"
        ))
        
        # Analyze path characteristics
        successful_hops = [h for h in hops if h.get('status') == 'OK' and h.get('latency')]
        
        if len(successful_hops) < 2:
            console.print("[yellow]⚠️ Insufficient data for path analysis[/yellow]")
            return
        
        # Calculate path metrics
        latencies = [h['latency'] for h in successful_hops]
        avg_latency = sum(latencies) / len(latencies)
        max_latency = max(latencies)
        min_latency = min(latencies)
        
        # Detect potential issues
        issues = []
        if max_latency > 100:
            issues.append("High latency detected (>100ms)")
        
        if len(successful_hops) < len(hops) * 0.8:
            issues.append("Packet loss detected (>20%)")
        
        # Display analysis
        table = Table(title="📊 Path Metrics", box=box.ROUNDED)
        table.add_column("Metric", style="cyan")
        table.add_column("Value", style="green")
        
        table.add_row("Total Hops", str(len(hops)))
        table.add_row("Successful Hops", str(len(successful_hops)))
        table.add_row("Average Latency", f"{avg_latency:.1f}ms")
        table.add_row("Min Latency", f"{min_latency:.1f}ms")
        table.add_row("Max Latency", f"{max_latency:.1f}ms")
        table.add_row("Packet Loss", f"{((len(hops) - len(successful_hops)) / len(hops) * 100):.1f}%")
        
        console.print(table)
        console.print()
        
        # Display issues if any
        if issues:
            console.print("[red]🚨 Potential Issues:[/red]")
            for issue in issues:
                console.print(f"  • {issue}")
            console.print()
        
        # Network topology insights
        console.print("[blue]💡 Network Insights:[/blue]")
        if len(hops) <= 5:
            console.print("  • Direct or near-direct connection")
        elif len(hops) <= 10:
            console.print("  • Standard internet routing")
        else:
            console.print("  • Complex routing path")
        
        if avg_latency < 20:
            console.print("  • Excellent connection quality")
        elif avg_latency < 50:
            console.print("  • Good connection quality")
        else:
            console.print("  • Moderate to high latency")
        
        console.print() 