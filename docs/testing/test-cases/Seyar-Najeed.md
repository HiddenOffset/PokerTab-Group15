# Test Cases: Seyar Najeed

Team number: 1 My area: Player Management

---

Test ID: SN-01 From Part 6: TC-2 Written by: Seyar Najeed Title: Removing a player who has no buy-ins

Traces to: REQ-SN-02 Test type: Unit, Positive

Preconditions: A session has been created and a player named Test has been added without any buy-ins.

Test data: Player name: Test

Steps:
1. Create a session.
2. Add a player named Test.
3. Try removing the player from the session.

Expected result: The player is removed from the session.

Actual result: Not run yet.

Status: Not run Severity: Minor V or V: Verification GitHub Issue: N/A

---

Test ID: SN-02 From Part 6: TC-1 Written by: Seyar Najeed Title: Trying to remove a player who has a buy-in

Traces to: REQ-SN-02 Test type: Unit, Negative

Preconditions: A session has been created and a player named Test has already made a buy-in.

Test data: Player name: Test, Buy-in: $20.00

Steps:
1. Create a session.
2. Add a player named Test.
3. Give the player a $20.00 buy-in.
4. Try removing the player.

Expected result: The player should not be removed because they have a transaction.

Actual result: Not run yet.

Status: Not run Severity: Major V or V: Verification GitHub Issue: N/A

---

Test ID: SN-03 From Part 6: New Written by: Seyar Najeed Title: Checking if the roster updates after removing a player

Traces to: REQ-SN-02 Test type: Integration

Preconditions: A session is open and has a player named Test with no buy-ins.

Test data: Player name: Test

Steps:
1. Open a poker session.
2. Add a player named Test.
3. Remove the player from the dashboard.
4. Look at the player roster.

Expected result: Test should no longer show up in the player roster.

Actual result: Not run yet.

Status: Not run Severity: Major V or V: Validation GitHub Issue: N/A

---

Test ID: SN-04 From Part 6: New Written by: Seyar Najeed Title: Removing a player from the poker application

Traces to: REQ-SN-02 Test type: Acceptance

Preconditions: A poker session is open and has a player named Test with no buy-ins.

Test data: Player name: Test

Steps:
1. Open the poker application.
2. Open a session.
3. Add a player named Test.
4. Click the Remove option next to the player.
5. Check the roster to see if the player is gone.

Expected result: The player is removed and no longer appears in the roster.

Actual result: Not run yet.

Status: Not run Severity: Major V or V: Validation GitHub Issue: N/A
