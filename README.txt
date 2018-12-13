20/11
cSoundMgr:
- fixed bug with crackling music

cRocket:
- added A nad D keys for movement

cGameStuVer:
- positioned properly texts
- added all textures
- added fonts (Berlin and Freshman)
- cleaned code from unused functions

27/11
cGameStuVer:
- added walls on the left, right and top side of the screen
- fixed bug with two textures giving fatal errors
- bullets dont disappear after they colide now
- bullets bounce of the platform

cRocket:
- added the wall width to file, platform cannot move onto the wall

cBulletStuVer, cBullet:
- added the walls' dimensions to the file so that the ball can bounce from it

28/11
cGameStuVer:
- designed the first level, bricks dont fly anymore
- bricks have health points and when it reaches 0 they vanish
- bricks change texture when getting damaged 
- added damaged bricks textures
- added bug with brick_orange_dmg2 texture
- fixed the added bug with the texture :)

Brick images:
- changed width to 75 pixels

Wall images: 
- changed width to 40 pixels (right and left walls)
- changed height to 40 pixels (top wall)

29/11
Project:
- added XInput.lib

cGameStuVer:
- designed third level
- added points for specific destroyed bricks (10 for white, 25 for green and so on)
- added a timer
- added 3 heart textures that represent player's lifes
- added main and end menu
- added text to main and end menu
- started adding buttons

6/12
Game rules now change, the quicker the player finishes the more points he gets (the player doesn't lose when the time runs out), for every lost life his points are taken.

cGameStuVer:
- limited the amount of balls at once (1)
- added lifes (max of 3)
- lifes can be lost now
- player will be notified to press 'space' to shoot the ball out
- added sound when player loses life
- added sound when ball hits the walls
- implemented menu, level, and end screen
- added a new hit effect
- implemented new sounds

7/12
Project:
- added highscore cpp and header files

cGameStuVer:
- added highscore display at the end of game

8/12 
cHighScoreTalbe:
- changed nearly the entire file to suit own needs

cGameStuVer:
- added a highscore at the end screen

9/12
Created cController cpp and header files to implement control by gamepad

cRocket:
- added XInput
- added controls to support gamepad (moving with left stick)

cGameStuVer:
- added controls to shoot the ball at the start of the game with the gamepad's 'A' button
- game ends now when user loses his lifes
