# V&V Sprint: Test the System You Inherited

On Halloween you tested a stranger's app against its requirements. Now do it for real, on the project you inherited at the handoff.

You already did the first draft. In `docs/SYSTEM_MAP.md` you wrote:
- **Part 5:** candidate requirements, each tagged Confirmed, Unconfirmed, or Suspected bug
- **Part 6:** three unit test cases

This sprint **upgrades** that work. You are not starting over. Part 5 becomes a categorized, testable requirements list. Part 6 becomes a full test suite that uses every test type from the Field Guide. Every failure becomes a **GitHub Issue**.

**Do not edit Parts 5 and 6 in `SYSTEM_MAP.md`.** Leave them as your first draft. The new versions live in `docs/testing/`, and each row points back to where it came from.

---

## Step 0: Put this folder in your repo (team captain, 2 minutes)

1. Copy this whole `testing/` folder into the `docs/` folder of your inherited repo, next to `SYSTEM_MAP.md`.
2. Commit it to `main` through a pull request, like any other change.

When you are done, your repo should look like this:

```
your-inherited-repo/
|-- docs/
    |-- SYSTEM_MAP.md                   <- already done, do not edit Parts 5 and 6
    |-- testing/
        |-- README.md                   <- this file
        |-- TESTING_FIELD_GUIDE.md      <- same guide from the bug hunt
        |-- REQUIREMENTS.md             <- Part 1 (everyone adds their section)
        |-- TRACEABILITY.md             <- Part 4 (team)
        |-- test-cases/
            |-- TEMPLATE.md             <- do not edit
            |-- maria-g.md              <- Part 2 (one file per student)
```

---

## Step 1: Split the system four ways (team, 5 minutes)

Open `SYSTEM_MAP.md`. Divide the system into four areas, one per person. Example: accounts and login, the main feature, data storage, and the user interface. Write the split at the top of `REQUIREMENTS.md`.

Then hand out your Part 5 requirements and Part 6 tests. Each one goes to the person whose area it belongs to. Some people will get more than others. That is fine.

You own your area for the whole sprint. Your requirements, your test cases, and your issues all come from it.

---

## Part 1: Upgrade Your Part 5 Requirements (individual)

Part 5 was a first draft: "The system shall..." statements with a type, a source, a status, and an acceptance criterion. Some are still Unconfirmed. Now you finish the job.

In `REQUIREMENTS.md`, under **your own section**:

1. Bring in the Part 5 requirements that belong to your area. Keep their original IDs (CR-1, CR-2, and so on) in the "From Part 5" column.
2. **Settle the status.** Read the code and test the running system. Every **Unconfirmed** must become **Confirmed** or **Suspected bug**. Every **Suspected bug** becomes a GitHub Issue in Part 3.
3. **Sort each one into a category**, exactly like the Fright Night requirements doc:
   - **Stakeholder Goal (G)**: what a real person needs. Not testable by itself, but it is how you find Validation bugs.
   - **Functional Requirement (REQ)**: something the system does.
   - **Non-Functional Requirement (NFR)**: how well it does it. Speed, security, usability.
4. **Rewrite any requirement that cannot be tested.** The "From Part 5" column keeps the original, so we can see the change.
5. **Fill the gaps.** Part 5 had no Stakeholder Goals at all, and most teams wrote only one Non-Functional requirement. Add them. Ask: who uses this area, and what do they actually need? How fast, how secure, how easy?
6. Give each one a new ID with your initials: `REQ-MG-01`, `NFR-MG-01`, `G-MG-01`.

**Minimum:** at least 1 Goal, 3 REQs, and 1 NFR in your section. Part 5 requirements count toward the 3 REQs. Write "New" in the "From Part 5" column for anything you add.

**The test for a good requirement:** could a stranger write a Pass or Fail test for it without asking you a question? If not, rewrite it.

---

## Part 2: Upgrade Part 6 Into a Full Test Suite (individual)

Part 6 gave your team three unit tests (positive, negative, and boundary) with a predicted result. You never ran them. Real systems also need more than unit tests. The Field Guide has seven types, and you used all of them on Halloween.

1. Copy `test-cases/TEMPLATE.md` to a new file named with your first name and last initial, like `test-cases/maria-g.md`.
2. **If a Part 6 test belongs to your area, upgrade it first.** Move it into your file and fill in every field from the Field Guide anatomy: Traces To (now your new REQ ID), Test Type, Preconditions, exact Test Data, Actual Result, Status, Severity, and V or V. Write its old Part 6 number in the "From Part 6" field. Your Part 6 "Predicted result" becomes a real Actual Result once you run it.
3. **Then write new tests** against your own requirements, using types Part 6 did not cover. Use the format from `TESTING_FIELD_GUIDE.md`. TC-00 is your model.
4. **Actually run every test** on the inherited system. Fill in Actual Result and Status.

**Minimum:**
- At least **4 test cases**. Upgraded Part 6 tests count.
- At least **3 different test types** from the Field Guide
- At least **1 Acceptance test** that traces to your Stakeholder Goal

A test that passes is still a good test. Not every test needs to fail.

---

## Part 3: Log Every Failure as a GitHub Issue (individual)

Open a GitHub Issue for:
- every test case marked **Fail**
- every requirement in your section still marked **Suspected bug**

**Issue title:** `[Test ID] One-line description of the bug`
Example: `[MG-03] Search returns results for an empty query`

**Issue body:** paste the full test case.

**Labels** (captain creates these once, under Issues > Labels):

| Label | Use it when |
|---|---|
| `verification` | The system does not match the requirement |
| `validation` | The system matches the requirement, but the requirement does not serve the Goal |
| `critical` / `major` / `minor` | Severity, from the Field Guide |

Add each issue to your GitHub Projects board in the **Todo** column. Assign it to yourself.

**Do not fix the bugs this sprint.** Finding them is the job. Fixing them is next sprint.

---

## Part 4: Traceability Matrix (team)

Fill in `TRACEABILITY.md`. Every requirement gets a row. Every row lists the test cases that check it.

A requirement with **no test** is a gap. A test with **no requirement** is testing vibes. The matrix shows both at a glance.

**Team minimum:**
- Every REQ and NFR has at least 1 test case
- All **7 test types** appear somewhere across the team
- At least **1 Validation finding**, or 2 to 3 sentences explaining why every requirement truly serves its Goal

---

## How to Commit

1. Work on **your own branch**, named with your first name.
2. Only edit **your own** test case file and **your own** section of `REQUIREMENTS.md`.
3. Open a pull request. A teammate approves it before it merges.
4. The captain fills in `TRACEABILITY.md` last, after everyone's work is merged.

Because everyone edits different files or different sections, you should not get merge conflicts. If you do, tell your team before you resolve it.

---

## Checklist Before You Submit

**Me:**
- [ ] My section of `REQUIREMENTS.md` has at least 1 Goal, 3 REQs, 1 NFR, with IDs and categories
- [ ] Every Part 5 requirement in my area is brought in, with no Unconfirmed left
- [ ] Any vague requirement is rewritten, with the Part 5 version kept
- [ ] Any Part 6 test in my area is upgraded with every field filled in
- [ ] My test case file has at least 4 tests and 3 types, including 1 Acceptance test
- [ ] Every test has an Actual Result and a Status
- [ ] Every Fail and every Suspected bug has a GitHub Issue, labeled and on the board
- [ ] My PR was approved and merged
- [ ] My developer log is done

**Team:**
- [ ] All Part 5 requirements and all 3 Part 6 tests were handed out to someone
- [ ] `TRACEABILITY.md` is complete
- [ ] Every requirement has a test
- [ ] All 7 test types used
- [ ] At least 1 Validation finding or explanation
