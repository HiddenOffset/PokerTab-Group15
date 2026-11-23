**Name of person who fixed bug:** Seyar Najeed

**Bug that was fixed #95:** Session name overrides previous work

-Issue: [GitHub Issue #95](https://github.com/HiddenOffset/PokerTab-Group15/issues/64)
-Pull Request: 
-Severity: [Medium]
-Priority: [Medium]

#Before the fix
![before the fix] <img width="1919" height="880" alt="seyar_najeed_issue95_before(1-2)" src="https://github.com/user-attachments/assets/0ec900b7-d241-435e-88da-d70e45e67dc9" />
<img width="1919" height="1079" alt="seyar_najeed_issue95_before(2-2)" src="https://github.com/user-attachments/assets/16af21bf-a318-4136-9d7a-0cd2252fb097" />




#After the fix 
![after the fix] <img width="1270" height="826" alt="seyar_najeed_issue95_after(1-2)" src="https://github.com/user-attachments/assets/369b01d4-8401-4962-925a-b3b94fd6a4b0" />
<img width="604" height="391" alt="seyar_najeed_issue95_after(2-2)" src="https://github.com/user-attachments/assets/db07824a-f710-47e9-b867-beec042c0fc4" />



**Explanation:** The bug caused a new session with the same name to overwrite the previous session instead of creating a new one. I tested this by creating another session with the same name and checking what happened to the original session.

**AI Disclosure:** I used Claude to help me write mock tests to help me test the fix for my bug.
