**Name of person who fixed bug:** Seyar Najeed

**Bug that was fixed #95:** Session name overrides previous work

-Issue: [GitHub Issue #95] (https://github.com/HiddenOffset/PokerTab-Group15/issues/95)
-Pull Request: 
-Severity: [Medium]
-Priority: [Medium]

#Before the fix
<img width="1917" height="1033" alt="seyar_najeed_issue95_before" src="https://github.com/user-attachments/assets/11237dc3-ae59-4d16-a609-ed9463106920" />






#After the fix 
<img width="1919" height="1011" alt="seyar_najeed_issue95_after" src="https://github.com/user-attachments/assets/d7ab6bdc-feb9-45d8-b61b-31a91f38ecab" />



**Explanation:** The bug #95 originally #64 caused a new session with the same name to overwrite the previous session instead of creating a new one. I tested this by creating another session with the same name and checking what happened to the original session.

**AI Disclosure:** I used Claude to help me write mock tests to help me test the fix for my bug.
