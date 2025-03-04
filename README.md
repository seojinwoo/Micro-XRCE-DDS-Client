# Micro XRCE-DDS Client

[![Releases](https://img.shields.io/github/release/eProsima/Micro-XRCE-DDS-Client.svg)](https://github.com/eProsima/Micro-XRCE-DDS-Client/releases)
[![License](https://img.shields.io/github/license/eProsima/Micro-XRCE-DDS-Client.svg)](https://github.com/eProsima/Micro-XRCE-DDS-Client/blob/master/LICENSE)
[![Issues](https://img.shields.io/github/issues/eProsima/Micro-XRCE-DDS-Client.svg)](https://github.com/eProsima/Micro-XRCE-DDS-Client/issues)
[![Forks](https://img.shields.io/github/forks/eProsima/Micro-XRCE-DDS-Client.svg)](https://github.com/eProsima/Micro-XRCE-DDS-Client/network/members)
[![Stars](https://img.shields.io/github/stars/eProsima/Micro-XRCE-DDS-Client.svg)](https://github.com/eProsima/Micro-XRCE-DDS-Client/stargazers)
[![Read the Docs](https://img.shields.io/readthedocs/micro-xrce-dds?style=flat)](https://micro-xrce-dds.docs.eprosima.com/en/latest/)
[![Twitter Follow](https://img.shields.io/twitter/follow/eprosima?style=social)](https://twitter.com/EProsima)

[![Docker Build Status](https://img.shields.io/docker/cloud/build/eprosima/micro-xrce-dds-client)](https://hub.docker.com/r/eprosima/micro-xrce-dds-client/)

<a href="http://www.eprosima.com"><img src="https://encrypted-tbn3.gstatic.com/images?q=tbn:ANd9GcSd0PDlVz1U_7MgdTe0FRIWD0Jc9_YH-gGi0ZpLkr-qgCI6ZEoJZ5GBqQ" align="left" hspace="8" vspace="2" width="100" height="100" ></a>

_eProsima Micro XRCE-DDS_ is a library implementing the [DDS-XRCE protocol](https://www.omg.org/spec/DDS-XRCE/About-DDS-XRCE/) as defined and maintained by the OMG, whose aim is to allow resource constrained devices such as microcontrollers to communicate with the [DDS](https://www.omg.org/spec/DDS/About-DDS/>) world as any other DDS actor would do.
It follows a client/server paradigm and is composed by two libraries, the _Micro XRCE-DDS Client_ and the _Micro XRCE-DDS Agent_. The _Micro XRCE-DDS Clients_ are lightweight entities meant to be compiled on e**X**tremely **R**esource **C**onstrained **E**nvironments, while the _Micro XRCE-DDS Agent_ is a broker which bridges the _Clients_ with the DDS world.

<p align="center"> <img src="https://github.com/eProsima/Micro-XRCE-DDS-Client/blob/master/docs/General.png?raw=true" alt="General architecture" width="70%"/> </p>

The _Micro XRCE-DDS Clients_ request operations to the _Agent_ to publish and/or subscribe to topics in the DDS global dataspace. Remote procedure calls, as defined by the [DDS-RPC standard](https://www.omg.org/spec/DDS-RPC/About-DDS-RPC/), are also supported, allowing _Clients_ to communicate in the DDS dataspace according to a request/reply paradigm.
The _Agents_ process these requests and send back a response with the operation status result and with the requested data, in the case of subscribe/reply operations.
The communication in the DDS world is mediated by a dedicated `ProxyClient` in charge of creating the _DDS Entities_ requested by the _Clients_, such as _Participants_, _Topics_, _Publishers_, and _Subscribers_, which can interact with the DDS Global dataspace.

<p align="center"> <img src="https://github.com/eProsima/Micro-XRCE-DDS-Client/blob/master/docs/Client.png?raw=true" alt="Client architecture" width="70%"/> </p>

_eProsima Micro XRCE-DDS_ provides the user with a C API to create _Micro XRCE-DDS Clients_ applications. The library can be configured at compile-time via a set of CMake flags allowing to enable or disable some profiles before compilation, and to manipulate several parameters controlling some of the library's functionalities, which in turn allow tuning the library size.

The communication between a _Micro XRCE-DDS Client_ and a _Micro XRCE-DDS Agent_ is achieved by means of several kinds of built-in transports: **UDPv4**, **UDPv6**, **TCPv4**, **TCPv6** and **Serial** communication. In addition, there is the possibility for the user to generate its own **Custom** transport.

## Documentation

You can access the _eProsima Micro XRCE-DDS_ user documentation online, which is hosted on Read the Docs.

- [Start Page](http://micro-xrce-dds.readthedocs.io)
- [Installation manual](http://micro-xrce-dds.readthedocs.io/en/latest/installation.html)
- [User manual](http://micro-xrce-dds.readthedocs.io/en/latest/introduction.html)

## Quality Declaration

**eProsima Micro XRCE-DDS Client** claims to be in the **Quality Level 1** category based on the guidelines provided by [ROS 2](https://ros.org/reps/rep-2004.html).
See the [Quality Declaration](QUALITY.md) for more details.

## Getting Help

If you need support you can reach us by mail at `support@eProsima.com` or by phone at `+34 91 804 34 48`.

## for Linux & Mac

cmake .. -DUCLIENT_BUILD_EXAMPLES=ON
make
./examples/Zigbang/Zigbang
