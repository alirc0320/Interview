# Etched interview prep — question map

Interview date: 2026-10-15. Source: Etched Accelerator Software C++ Interview
Question Guide.

## Coding problems (`problems/`)

Each folder has a blank `solution.h` (with the signature/types given in the
guide, where one was given), a blank `solution.cpp`, and a blank `test.cpp`.
Build with `make test PROB=<folder>`.

### Tier 1 — directly reported Etched questions (must finish)
| # | Folder | Question |
|---|--------|----------|
| 1 | `01_dma_offset_read` | Arbitrary-offset DMA read wrapper |
| 2 | `02_scatter_gather_writer` | Scatter/gather DMA stream writer |
| 3 | `03_periodic_event_loop` | Periodic event loop |
| 4 | `04_bitfield_extract_insert` | Extract and insert a bitfield |
| 5 | `05_matmul` | Rectangular matrix multiplication |

### Tier 2 — derived from the Accelerator Software role
| # | Folder | Question | Must finish? |
|---|--------|----------|---------------|
| 6 | `06_ring_buffer` | Fixed-capacity command ring buffer | yes |
| 7 | `07_command_completion_queues` | Accelerator command/completion queues | yes |
| 8 | `08_dma_descriptor_validator` | DMA descriptor validator | yes |
| 9 | `09_page_allocator` | Page-aligned device-memory allocator | if time |
| 10 | `10_mmio_register_fields` | MMIO register-field API | (not in guide's completion order — do if time) |
| 11 | `11_boot_state_machine` | Firmware boot state machine | yes |
| 12 | `12_power_sequencing_scheduler` | Power-sequencing dependency scheduler | if time |
| 13 | `13_thermal_throttling` | Thermal throttling policy | if time |
| 14 | `14_telemetry_snapshot` | Atomic telemetry snapshot | if time |
| 15 | `15_firmware_update_receiver` | Firmware-update chunk receiver | if time |
| 16 | `16_interrupt_polling_api` | Interrupt vs polling completion API | if time |
| 17 | `17_sriov_resource_assignment` | SR-IOV virtual-function resource assignment | if time (likely a design follow-up, not first screen) |

### Tier 3 — supporting C/C++ systems drills
| # | Folder | Question | Must finish? |
|---|--------|----------|---------------|
| 18 | `18_memmove` | Implement memmove | yes |
| 19 | `19_binary_command_serialization` | Binary command serialization | yes |
| 20 | `20_arena_allocator` | Arena allocator | if time |
| 21 | `21_bounded_producer_consumer` | Thread-safe bounded producer/consumer queue | yes |
| 22 | `22_timeout_wraparound` | Timeout arithmetic with timer wraparound | (not in guide's completion order — do if time) |
| 23 | `23_diagnose_unsafe_cpp` | Diagnose unsafe C++ code | (not in guide's completion order — code-review exercise, see the folder's `solution.h`) |

### Tier 4 — Etched-tagged algorithm questions (fluency practice, weaker evidence)
| # | Folder | Question |
|---|--------|----------|
| 24 | `24_atoi` | String to Integer (atoi) |
| 25 | `25_find_k_closest_elements` | Find K Closest Elements |
| 26 | `26_maximum_subarray` | Maximum Subarray |
| 27 | `27_frequency_most_frequent_element` | Frequency of the Most Frequent Element |
| 28 | `28_subarray_sum_equals_k` | Subarray Sum Equals K |
| 29 | `29_single_number` | Single Number |
| 30 | `30_design_hit_counter` | Design Hit Counter |
| 31 | `31_course_schedule` | Course Schedule |
| 32 | `32_max_sum_distinct_subarray_k` | Maximum Sum of Distinct Subarrays With Length K |
| 33 | `33_kth_smallest_sorted_matrix` | Kth Smallest Element in a Sorted Matrix |

### Extra OS-mechanism coding exercises (not in the Etched guide)
Added to support the OS study routine below — code only when a section
naturally calls for it.
| Folder | Exercise |
|--------|----------|
| `os_page_replacement` | Page replacement simulator (FIFO/LRU), count page faults |
| `os_cpu_scheduler` | CPU scheduling simulator (FCFS/Round Robin), produce a timeline |
| `os_file_cache` | Simple LRU block/file cache |

### Demo (not from the guide)
| Folder | Purpose |
|--------|---------|
| `two_sum` | Fully worked example proving the build/test setup works |

## Study routines

- `learncpp/` — after each LearnCpp chapter: 5 from-memory questions
  (`learncpp/template_chapter.md`), then correct against the chapter. Every
  two chapters: one combined `practice.cpp` with 5+ `assert()`s
  (`learncpp/practice/template.cpp`).

### LearnCpp chapter index

Full LearnCpp.com sequence (chapters 0–28 plus optional chapters O and F, in
their real site order). `learncpp/chapters/<slug>.md` has 5 Q-only prompts
per chapter (answers left blank — fill them in from memory after closing the
tab). `learncpp/practice/<slug>.cpp` is the paired 2-chapter combined
program, scaffolded with a task description but no implementation.

Suggested priority for this interview (see chat for full reasoning): pointers/
references (12, 12b), bit manipulation (O), dynamic allocation (19), structs/
classes (13–15), move semantics/smart pointers (22), and virtual functions
(25) are the highest-yield chapters for an Accelerator Software interview —
do those first if short on time.

`12b_pointers_advanced_embedded` is not a real LearnCpp chapter — it's a
custom addition inserted right after 12 because raw-pointer questions
(pointer arithmetic, `void*`/`reinterpret_cast`, `volatile`/MMIO, alignment
and strict aliasing) are disproportionately likely in an embedded/
accelerator interview and don't live in any single LearnCpp chapter. Do it
directly after 12, before moving on to 13.

| # | Chapter file | Practice pair |
|---|---|---|
| 0 | `00_introduction_getting_started` | `00_01` |
| 1 | `01_cpp_basics` | ↑ |
| 2 | `02_functions_and_files` | `02_03` |
| 3 | `03_debugging` | ↑ |
| 4 | `04_fundamental_data_types` | `04_05` |
| 5 | `05_constants_and_strings` | ↑ |
| 6 | `06_operators` | `06_o` |
| O | `o_bit_manipulation` | ↑ |
| 7 | `07_scope_duration_linkage` | `07_08` |
| 8 | `08_control_flow` | ↑ |
| 9 | `09_error_detection_handling` | `09_10` |
| 10 | `10_type_conversion_aliases_deduction` | ↑ |
| 11 | `11_function_overloading_templates` | `11_f` |
| F | `f_constexpr_functions` | ↑ |
| 12 | `12_references_and_pointers` | `12_13` |
| 12b | `12b_pointers_advanced_embedded` (custom, not real LearnCpp) | `12b` (solo) |
| 13 | `13_enums_and_structs` | ↑ |
| 14 | `14_introduction_to_classes` | `14_15` |
| 15 | `15_more_on_classes` | ↑ |
| 16 | `16_std_vector` | `16_17` |
| 17 | `17_std_array_c_arrays` | ↑ |
| 18 | `18_iterators_and_algorithms` | `18_19` |
| 19 | `19_dynamic_allocation` | ↑ |
| 20 | `20_functions` | `20_21` |
| 21 | `21_operator_overloading` | ↑ |
| 22 | `22_move_semantics_smart_pointers` | `22_23` |
| 23 | `23_object_relationships` | ↑ |
| 24 | `24_inheritance` | `24_25` |
| 25 | `25_virtual_functions` | ↑ |
| 26 | `26_templates_and_classes` | `26_27` |
| 27 | `27_exceptions` | ↑ |
| 28 | `28_io_streams` | `28` (solo) |

Appendices A–D are skipped (reference/miscellaneous material, not core
learning content).
- `os/` — after each OS chapter/section: explain the mechanism aloud, draw
  it, answer 5 questions (`os/template_section.md`), then correct. Code only
  when it fits (see the OS exercises above).
- `review/` — `review/missed_questions.md` tracks anything missed until you
  can explain it unprompted; `review/log.md` is the dated log for the
  next-day / weekend / following-week review cadence.

See the top-level `README.md` for commands.
