Notes and limitations
Unfortuneately, encryption of passwords or data is still a thing of the future, not of this demo program.
Similarly, the password entry field is not visually obscured, so anyone observing your screen can see it when entered.
As a last security concern, while a user is kicked out of the program if they make too many unsuccessful login attempts, there is nothing currently to prevent them from simply launching the program again. I'm hesitant to go much further in this area, as it seems like it warrants negotiation between systems administrators and the design specification team.

At present, this program only supports a single user and authorization level, however, the structure of the data storage would make it easy to add additional users and additional authorization levels at any time.
All actions taken while using this software are logged and recorded in the provided banklog.txt. While an unscrupulous individual could delete this log to hide activities, the destination of the banklog can easily be moved to a more secure location if one is available.

