# Clone input into the pointer to the right. Works from any pointer and will go from it but it will wipe the two slots to the right so beware

# Clear Two to right
>[-]
>[-]
<<

# Add C0 to C1 and C2
[
	-
	>+
	>+
	<<
]

# Move C2 to C0
>>
[
	-
	<<+
	>>
]
<<
# Done