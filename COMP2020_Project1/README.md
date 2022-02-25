# CECS2020-Project-1---Khau-Lien-Kiet
Name: Khau Lien Kiet (V202000119)
Email: 20kiet.kl@vinuni.edu.vn
# The critical path length of the complete ALU estimation: 
Objective of counting:
    -Standard AND, OR, NOR, NAND, XOR, and XNOR gates have a gate delay of one unit
    -NOT gates are ignored
    -Multi-input and multi-bit gates both have the same gate delay as their standard variants
    -A mux has a gate delay of 2 regardless of size 
=> Number: About 70 paths
# The number of gates required to implement the ALU (including gates needed for subcomponents) estimation:
Objective of counting:
    - Standard AND, OR, NOR, NAND, XOR and NXOR gates count as one gate
    - NOT gates are ignored
    - Multi-input gates count as a single gate
    - An n-bit gate counts as n gates (the multi-bit gates Logisim provides aren't actually real; they are just a convenient shorthand for using a gate for each bit)
    - A mux counts as (# of data bits)*(# of inputs + 1) gates 
=> Number: More than 1410 gates
    
# CECS_COMP2020
