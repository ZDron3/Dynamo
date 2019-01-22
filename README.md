# Dynamo
BomberManGameTestC++

A classic bomber game as part of a c++ assginment, it consists of the following gameplay mechanics <br/><br/>

1] A Procedural Level Generation handled by a LevelManager.<br/>
2]Local Multiplayer where the player's can play using the same keyboard (Supports 2 players)<br/>
3]Top Down camera view which moves according the distance between the player <br/>
4]Power-ups which are spawned when the wall is destroyed by the player, the spawnrate of the power-up is around 25% which can be manipulated, the game consists of follow power-ups<br/>
a.RangePower Up giving longer blast range (linear)<br/>
b.More than one bomb spawn which is the base implement of the bomb spawns by character each powerup pickup increase the bomb spawn count by 1<br/>
c.Faster running speed <br/>
d.Remote controlled bombs powerup where the explosion of the bomb is delayed until the player trigger's it ,each pickup give one remote controlled bomb<br/><br/>
 
 Bomb blast is linear in four directions,stopped by walls,kill player and power-ups,trigger other bombs.<br/>

## Inputs <br/>
###### Player 1 <br/>
 WASD for Movement <br/>
 LShift for bomb spawn <br/>
 LCtrl for triggering remote controlled bombs<br/>
 <br/><br/>
###### Player 2 <br/>
 Direction Keys for Movement <br/>
 RShift for bomb spawn <br/>
 RCtrl for triggering remote controlled bombs<br/>
 <br/><br/>
 A .rar file can be found in the repository which consists the build file for the game, you can extract the file and run the .exe<br/>
