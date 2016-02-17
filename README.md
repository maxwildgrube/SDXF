# SDXF
Structured Data eXchange Format


1. Introduction

SDXF is a data format which is also published as an Internet RFC (see http://www.ietf.org/rfc/rfc3072.txt)

The purpose of this format: arbitrary structured data of different types are assembled together for exchanging between computers of different architectures.

With SDXF you can structure your data in any levels of deepness. The particular data elements (called "Chunks") are self-describing. The format is very simple, but nevertheless transparent to the programmer, he does not need and should not care about the bits and bytes of the structure. To access and create the structure the programmer uses a set of functions. But "not to care" means not "not to know": He has already to know how SDXF works.

The exchanging of data between computers of different archicture is realized by a transparently transformation of the data to a normalized (network) form. This is done by the set of SDXF-functions.

May be the imagination of a SDXF data structure as an administration buildung will be helpful:
This building has a main entrance, you "enter" (this is the name of a SDXF-function) the building and you come into a corridor with some doors: behind some doors there is a bureau of a clerk, behind others there are other corridors with doors (which can also conceal corridors and doors, and so on…). On every door the name of the clerk or the indication of the corridor is to read, and: you can distinguish between a door and a corridor.
So you can walk from one door to the "next" (this is another name of a SDXF-function). If you open a door of a corridor you can "enter" it, too. If you open a door of a bureau you can "extract" (yes, also a function) information from the clerk (I hope).

Additionally to the adaption of the data, there are two other functions which will work in the background while using the SDXF functions:

    compressing and decompressing
    encryption and decryption

You can use SDXF also as a file format, maybe as a basic for a data base.

The possibility to structure your data in arbitrary deepness remembers somehow of XML, but SDXF is not a text format (as XML), you cannot manipulate a SDXF structure with a text editor. If nevertheless there is a need to do so, please have a look on SDEF.
