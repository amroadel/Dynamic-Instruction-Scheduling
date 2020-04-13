# Data Structures
- instruction
- circular FIFO (fake ROB)
- dispatch_list
- issue_list
- execute_list
- register file (if needed)

# Enums
- Instruction state

# Functions
- FakeRetire()
- Execute()
- Issue()
- Dispatch()
- Fetch()
- Advance_Cycle()
- parser
- printer

# Output
- Timing information for every instruction as described in 7.3
- Total number of instructions
- Total number of cycles
- IPC