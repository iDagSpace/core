The Dagger (iDAG Space) cryptocurrency
================================

- Community site: http://idag.space  
- The Main net was launched May 5, 2018 at 10:00 GMT  


Principles:
----------

- Directed acyclic graph (DAG), not blockchain  
- Block = transaction = address  
- Original idea and implementation  
- Mineable, no premine, no ICO  
- Mining new money every 64 seconds  


Main chain idea:
---------------

Every block in DAG has up to 15 links to another blocks (inputs and outputs).
Block B is _referenced_ by another block A if we can reach B from A by following the links.
_Chain_ is a sequence of blocks each of which is referenced by the previous block.
Chain is called _distinct_ if every its block belongs to separate 64-seconds interval.
_Difficulty_of_block_ is 1/hash where _hash_ is sha256(sha256(block)) regarded as little-endian number.
_Difficulty_of_chain_ is sum of difficulties of blocks.
_Main_chain_ is the distinct chain with maximum difficulty.
Blocks in main chain are called _main_blocks_.

Daggers are mined in every main block.
For first 4 years 1024 iDAG are mined in each main block.
For second 4 years - 512 iDAG, and so on.
So, maximum iDAG supply is approximately power(2,32).
Each dagger is equal to power(2,32) cheatoshino.
Transaction is _valid_ if it is referenced by a main block.
Valid transactions are strictly ordered depending on main chain and links order.
Double spending is prohibited because only first concurrent transaction (by this order) is applied.


Structure of block:
------------------

_The on-disk format will change in the future. Consider this the network protocol._
Each block has a fixed size of 512 bytes.
Block consists of 16 fields each of which has length 32 bytes.
Field 0 is header, it consists of 4 quadwords:
- transport-layer header
- types of all 16 fields, 4 bits for one type
- timestamp of the block, in seconds from Unix era * 1024
- block fee in cheatoshi

Types of fields:

0. nonce
1. header
2. transaction input: 24 lower bytes of block hash and 8 bytes of input amount
3. transaction output, structure is the same as input
4. half of block signature; ECDSA number r or s; digest for signature is hash of (block concate public key)
5. half of output signature; only owner of this key can use this block as input
6. public key (x) with even y
7. public key with odd y
8. ... 15. are reserved for future usage.


Transport layer:
---------------

The dnet network is used as transport layer.
_A new transport layer will come in the future._
