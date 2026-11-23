**Name of person who fixed bug:** Seyar Najeed

**Bug that was fixed #64:** Session name overrides previous work

-Issue: [GitHub Issue #64](https://github.com/HiddenOffset/PokerTab-Group15/issues/64)
-Pull Request: 
-Severity: [Medium]
-Priority: [Medium]

#Before the fix
![before the fix]<img width="1919" height="1079" alt="seyar_najeed_issue64_before(2-2)" src="https://github.com/user-attachments/assets/5b42ab9d-8a22-48b7-b791-7eda71774981" />
<img width="1919" height="880" alt="seyar_najeed_issue64_before(1-2)" src="https://github.com/user-attachments/assets/4714d7f0-f300-4e9d-9c72-3977605b7a26" />


#After the fix
![after the fix] <img width="604" height="391" alt="seyar_najeed_issue64_after(2-2)" src="https://github.com/user-attachments/assets/b0a7453b-afd1-4189-810c-6a805beb4fe9" />
<img width="1270" height="826" alt="seyar_najeed_issue64_after(1-2)" src="https://github.com/user-attachments/assets/bb6c93bf-fb76-4a14-b4d5-621d519c3336" />


**Explanation:** The bug caused a new session with the same name to overwrite the previous session instead of creating a new one. I tested this by creating another session with the same name and checking what happened to the original session.

**AI Disclosure:** I used Claude to help me write mock tests to help me test the fix for my bug.
