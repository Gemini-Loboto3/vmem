=====================================================
= CD-rom cracktro-like boot                         =
=---------------------------------------------------=
= Gemini/Loboto3 2016                               =
=====================================================

This source comes from the following #psxdev article:
http://appleofeden.de-doc.com/index.php/2016/06/21/intro-coding-and-cd-rom-usage/

Sample code to create a simple cracktro-like boot.
Also includes:
- Full source to load files from disk from LibCD
  with async byte-precise reads
- Particle effects applied to model geometry with
  inline GTE code to transform single vertices
- Inline GTE code to handle double-faced meshes
- Basic SPU initializer to kill BIOS boot reverb
- A packer to build memory and disk container files
- Sprite code for font with scaling and wave effects
