#topologies in a txt file

online [gogle-ppt](https://docs.google.com/presentation/d/18gcPZfQhGVh_5p10-eJJy8CWcNY-CwjHvXHsdGcqbFA/edit?usp=sharing) for the graphic-representation of the topologies

* topologies are stored as txtfiles
    * file name must be t#.txt, where # is a single character to identify the topologie
    * each line represents a link
        * format: `[node 1 ip-adrrs]~[node 2 ip-adrrs]~[ink cost]`
    * all links are bidirectional; therefore is only necessary to have it once in the txt file
        * multiple apereances of the same link have undefined behavior in the final program

please add a image representing the topologie 