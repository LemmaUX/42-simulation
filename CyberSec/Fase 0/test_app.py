#!/usr/bin/env python3
"""
Test script for Packet Odyssey
Verifies that all modules work correctly
"""

import sys
import os

# Add current directory to path for imports
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

def test_imports():
    """Test that all modules can be imported"""
    print("🔍 Testing module imports...")
    
    try:
        from layers import PacketEncapsulator
        print("✅ layers.py imported successfully")
    except ImportError as e:
        print(f"❌ Failed to import layers.py: {e}")
        return False
    
    try:
        from dns_resolver import DNSResolver
        print("✅ dns_resolver.py imported successfully")
    except ImportError as e:
        print(f"❌ Failed to import dns_resolver.py: {e}")
        return False
    
    try:
        from tracer import Traceroute
        print("✅ tracer.py imported successfully")
    except ImportError as e:
        print(f"❌ Failed to import tracer.py: {e}")
        return False
    
    try:
        from capture import PacketCapture
        print("✅ capture.py imported successfully")
    except ImportError as e:
        print(f"❌ Failed to import capture.py: {e}")
        return False
    
    try:
        from main import PacketOdyssey
        print("✅ main.py imported successfully")
    except ImportError as e:
        print(f"❌ Failed to import main.py: {e}")
        return False
    
    return True

def test_dns_simulation():
    """Test DNS simulation functionality"""
    print("\n🔍 Testing DNS simulation...")
    
    try:
        from dns_resolver import DNSResolver
        resolver = DNSResolver()
        ip = resolver.simulate_dns_query("example.com")
        print(f"✅ DNS simulation successful: example.com → {ip}")
        return True
    except Exception as e:
        print(f"❌ DNS simulation failed: {e}")
        return False

def test_encapsulation():
    """Test packet encapsulation"""
    print("\n🔍 Testing packet encapsulation...")
    
    try:
        from layers import PacketEncapsulator
        encapsulator = PacketEncapsulator()
        data = encapsulator.encapsulate_packet("example.com", "93.184.216.34")
        print("✅ Packet encapsulation successful")
        return True
    except Exception as e:
        print(f"❌ Packet encapsulation failed: {e}")
        return False

def test_traceroute_simulation():
    """Test traceroute simulation"""
    print("\n🔍 Testing traceroute simulation...")
    
    try:
        from tracer import Traceroute
        tracer = Traceroute()
        hops = tracer.trace_route("example.com", simulate=True)
        print(f"✅ Traceroute simulation successful: {len(hops)} hops")
        return True
    except Exception as e:
        print(f"❌ Traceroute simulation failed: {e}")
        return False

def test_packet_capture_simulation():
    """Test packet capture simulation"""
    print("\n🔍 Testing packet capture simulation...")
    
    try:
        from capture import PacketCapture
        capture = PacketCapture()
        packets = capture.capture_packets("example.com", simulate=True)
        print(f"✅ Packet capture simulation successful: {len(packets)} packets")
        return True
    except Exception as e:
        print(f"❌ Packet capture simulation failed: {e}")
        return False

def main():
    """Run all tests"""
    print("🚀 Packet Odyssey - Test Suite")
    print("=" * 50)
    
    tests = [
        test_imports,
        test_dns_simulation,
        test_encapsulation,
        test_traceroute_simulation,
        test_packet_capture_simulation
    ]
    
    passed = 0
    total = len(tests)
    
    for test in tests:
        if test():
            passed += 1
    
    print("\n" + "=" * 50)
    print(f"📊 Test Results: {passed}/{total} tests passed")
    
    if passed == total:
        print("🎉 All tests passed! Packet Odyssey is ready to use.")
        print("\nTo run the application:")
        print("  python main.py")
        print("\nFor simulation mode:")
        print("  python main.py --simulate --full-journey")
    else:
        print("⚠️ Some tests failed. Please check the error messages above.")
        print("\nTry installing dependencies:")
        print("  pip install -r requirements.txt")
    
    return passed == total

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1) 