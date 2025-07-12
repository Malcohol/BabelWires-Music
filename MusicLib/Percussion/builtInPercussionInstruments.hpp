/**
 * Defines an enum for the built-in percussion instruments.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicTypes.hpp>

#include <BabelWiresLib/Types/Enum/enumWithCppEnum.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>

#include <optional>

/// A master list of all the built-in percussion instruments (including SFX instruments that are used in
/// percussion kits on some instruments).
/// We assume the General MIDI standard v2 gives good coverage of percussion instruments, so we use the instruments
/// found there. Note that the notion of a percussion instrument is extensible: it is not required that a percussion
/// event have a value defined by this set.
#define BUILT_IN_PERCUSSION_INSTRUMENTS(X)                                                                             \
    /* Instruments of the GM2 Standard Kit */                                                                          \
    X(HighQ, "High Q", "22074fd9-40ed-4716-8e5b-4e174daa03b3")                                                         \
    X(Slap, "Slap", "3aedf5de-904b-46e2-bc87-445c9f065868")                                                            \
    X(ScrPsh, "Scratch Push", "3087699b-94bd-4612-b1bd-b5acf037c325")                                                  \
    X(ScrPll, "Scratch Pull", "8a74f73e-691d-445f-bd2e-fd75bc91a8ef")                                                  \
    X(Stcks, "Sticks", "c61880f7-2ed2-431b-b23a-14a5457bacfd")                                                         \
    X(SqClck, "Square Click", "4d887ce2-c748-4127-b3ed-f8430237a82f")                                                  \
    X(MtrClk, "Metronome Click", "a012ad55-b894-4687-8754-6574f492f90e")                                               \
    X(MtrBll, "Metronome Bell", "876abe67-81a4-41dd-9d7b-dbc3652c47a1")                                                \
    /* Instruments of the GM kit */                                                                                    \
    X(AcBass, "Acoustic Bass Drum", "6ce9c1ac-3439-41eb-9776-dac41184255c")                                            \
    X(Bass1, "Bass Drum 1", "1c39fd51-ed6b-439e-ac8e-3c0fc7cb0d3b")                                                    \
    X(SStick, "Side Stick", "7762bf9b-ee64-4af2-9397-f6a55751f845")                                                    \
    X(AcSnr, "Acoustic Snare", "86d0c201-7866-4d81-bf54-2417ff8d94b6")                                                 \
    X(Clap, "Hand Clap", "377aa8da-201c-4279-8db0-8bb1bfff48b3")                                                       \
    X(ElSnr, "Electric Snare", "d89ecda6-b911-47d3-b4a1-5bf24e358a52")                                                 \
    X(LFlTom, "Low Floor Tom", "1c125e5d-8764-4704-919b-a00a505d48bf")                                                 \
    X(ClHHat, "Closed Hi Hat", "594b33b1-61e6-44ee-877b-e12ff7309bf3")                                                 \
    X(HFlTom, "High Floor Tom", "df566de2-2e0d-4417-9ad0-a0225a28be81")                                                \
    X(PdHHat, "Pedal Hi Hat", "e79e3113-34da-4faa-ad71-fe6200c23a4b")                                                  \
    X(LwTom, "Low Tom", "4e0844b0-b79b-40ef-ae6d-8fbb8476799c")                                                        \
    X(OpHHat, "Open Hi Hat", "0fba2ee3-8223-4275-b4ac-06186824571d")                                                   \
    X(LMTom, "Low-Mid Tom", "8daf9af5-5839-4a60-a0fa-9ce7f8ddce23")                                                    \
    X(HMTom, "Hi-Mid Tom", "d05899d0-bee6-42ab-ae27-288bd7d35894")                                                     \
    X(Crash1, "Crash Cymbal 1", "19b94421-a2db-481c-a5c6-d760884e8b3f")                                                \
    X(HTom, "High Tom", "9a125d11-0376-4ae5-a4b0-8870fae943f6")                                                        \
    X(Ride1, "Ride Cymbal 1", "325540cc-bce6-4f4f-babd-89b56a48002d")                                                  \
    X(ChnCym, "Chinese Cymbal", "963bc408-32e7-4c33-a743-764ed26423d8")                                                \
    X(RideBl, "Ride Bell", "b0fa8c86-df41-4e4b-b9ce-e04a935f1db4")                                                     \
    X(Tamb, "Tambourine", "1c7b945f-d6c8-4213-a6c2-e15667399f9f")                                                      \
    X(SplCym, "Splash Cymbal", "3a630d65-0fb0-4272-ac30-ede579203e6d")                                                 \
    X(Cowbll, "Cowbell", "513e999a-40bf-4a3f-8164-c4b881300e4f")                                                       \
    X(Crash2, "Crash Cymbal 2", "b8604a23-c381-4cc5-aed4-fafa243a96f9")                                                \
    X(VibraS, "Vibraslap", "8948c9eb-31a1-4eb4-8b75-9b4a23c3b900")                                                     \
    X(Ride2, "Ride Cymbal 2", "266e41ce-c70c-4d51-8387-04025bf1b58d")                                                  \
    X(HBongo, "Hi Bongo", "a04a6551-b076-4bcc-a909-51ee8547e46c")                                                      \
    X(LBongo, "Low Bongo", "023c2b30-5616-42ce-93ca-0051b26487ba")                                                     \
    X(MHCnga, "Mute Hi Conga", "baaf8149-a3a4-479a-916f-cc34e8eaffc7")                                                 \
    X(OHCnga, "Open Hi Conga", "7188bb81-1e73-4c66-91c0-c9af3a8dcd38")                                                 \
    X(LConga, "Low Conga", "9c9eba72-632f-4a46-8c43-46425c0f8918")                                                     \
    X(HTimbl, "High Timbale", "da75f840-55a7-41c9-b74b-69c3ee16a299")                                                  \
    X(LTimbl, "Low Timbale", "fa81b07d-7c14-4331-8390-046166dc869f")                                                   \
    X(HAgogo, "High Agogo", "5b6f72e1-c491-42b4-b8b8-1e49648190fd")                                                    \
    X(LAgogo, "Low Agogo", "5709ee1f-50a0-44a2-a4e2-be2cf0b0affb")                                                     \
    X(Cabasa, "Cabasa", "feb12af2-bcaa-42cc-8325-ac5f73fdd835")                                                        \
    X(Maracs, "Maracas", "d824a9de-1c48-460d-b186-cdbe92329eaa")                                                       \
    X(SWhisl, "Short Whistle", "122a08b8-37cb-4caa-a12c-5ae4888475ac")                                                 \
    X(LWhisl, "Long Whistle", "f47406bb-47a1-47a1-b526-004ed56e0a16")                                                  \
    X(SGuiro, "Short Guiro", "cb801da6-8aad-4e29-bfed-83ed25490ff4")                                                   \
    X(LGuiro, "Long Guiro", "3bbd1626-0258-41d1-a84c-8c59939e3313")                                                    \
    X(Claves, "Claves", "76262dc4-7b66-4d8e-a68a-2ee29f675562")                                                        \
    X(HWoodB, "Hi Wood Block", "568178ca-43a8-454e-8415-02bceb1ecb36")                                                 \
    X(LWoodB, "Low Wood Block", "9e8ff317-af8c-4f99-a6d0-41a38ca6da96")                                                \
    X(MCuica, "Mute Cuica", "419d9a21-3b53-45f5-bf5a-fcdc72837708")                                                    \
    X(OCuica, "Open Cuica", "2e523d69-fea3-4145-96b7-1402f61472d5")                                                    \
    X(MTrian, "Mute Triangle", "8d72a586-72ee-48fe-9bbd-c1a3500ab648")                                                 \
    X(OTrian, "Open Triangle", "8ffa30b4-287f-4dcf-8d18-92def658e0d6")                                                 \
    /* Instruments of the GM2 Standard Kit */                                                                          \
    X(Shaker, "Shaker", "ee9c029a-98af-4679-adc4-3c5185ca2e95")                                                        \
    X(JngBll, "Jingle Bell", "4a3da07d-a820-44f0-98fc-8c409768f7c2")                                                   \
    X(BlTree, "Bell Tree", "770dd65c-6dd1-43ba-89f8-c4507c158709")                                                     \
    X(Cstnts, "Castanets", "f742bbe3-3679-4cbd-9727-5252fd1bfff1")                                                     \
    X(MSurdo, "Mute Surdo", "79286a74-fd6d-4918-8f4c-f79606e48d4e")                                                    \
    X(OSurdo, "Open Surdo", "16a9961e-26a2-4e85-bb8c-32b139713694")                                                    \
    /* Instruments from the GM2 Room kit */                                                                            \
    X(RLwTm2, "Room Low Tom 2", "d2c5e125-7853-46f8-bfd3-7dc707e62141")                                                \
    X(RLwTm1, "Room Low Tom 1", "e6b25f32-4a7e-4203-9aa3-76f0c6085ced")                                                \
    X(RMdTm2, "Room Mid Tom 2", "371bd6eb-33cb-40c8-ba57-3c18eac00787")                                                \
    X(RMdTm1, "Room Mid Tom 1", "58339244-fb5d-49d1-9403-8cf812388ab7")                                                \
    X(RHiTm2, "Room Hi Tom 2", "46d6b67b-9e66-46fb-ae5e-24b1296c856c")                                                 \
    X(RHiTm1, "Room Hi Tom 1", "a4b7fc45-3e90-4da4-b3b8-1e437d9a1e00")                                                 \
    /* Instruments from the GM2 Power kit */                                                                           \
    X(PKick, "Power Kick Drum", "326850a1-707f-4b6d-9940-0a9ca28d5a8b")                                                \
    X(PSnare, "Power Snare Drum", "26b107dc-8ad3-4875-a6ea-c7aff1d852ce")                                              \
    X(PLwTm2, "Power Low Tom 2", "90dddc4a-18d3-45ca-a942-f667a80a5963")                                               \
    X(PLwTm1, "Power Low Tom 1", "d8038c6c-5a57-404a-a819-f1f9aa152288")                                               \
    X(PMdTm2, "Power Mid Tom 2", "ad05c742-99a9-4ac1-98c3-163ba57458a4")                                               \
    X(PMdTm1, "Power Mid Tom 1", "a117afa9-9efa-4940-a856-f1f65352e25f")                                               \
    X(PHiTm2, "Power Hi Tom 2", "c0c794da-5353-46bb-9b65-a695ee8deae7")                                                \
    X(PHiTm1, "Power Hi Tom 1", "b18634b2-bdba-44be-8259-822b4b96be8a")                                                \
    /* Instruments from the GM2 Electronic Set */                                                                      \
    X(ElBass, "Electric Bass Drum", "d978c296-2e32-46d2-b8aa-ab00b31bdf36")                                            \
    X(ElSnr2, "Electric Snare 2", "4f7523a1-0996-4094-b5d7-c02ea5eb319a")                                              \
    X(ELwTm2, "Electric Low Tom 2", "2944dabe-ab8b-459c-974d-44fa453824c4")                                            \
    X(ELwTm1, "Electric Low Tom 1", "c2b2f01c-d428-46a3-8d38-fc090d6e4b7b")                                            \
    X(EMdTm2, "Electric Mid Tom 2", "9dd67d1c-cd29-4e9d-8df1-deb2e876cd60")                                            \
    X(EMdTm1, "Electric Mid Tom 1", "58bafd31-2bb3-4fb8-8d93-866d873279f1")                                            \
    X(EHiTm2, "Electric Hi Tom 2", "2956c899-57e2-4917-9064-5376cf9cfdef")                                             \
    X(EHiTm1, "Electric Hi Tom 1", "95d673c3-fdfa-4e06-bf34-997a5aeb1d2c")                                             \
    X(RevCym, "Reverse Cymbal", "6c81460a-88f6-484b-994f-5aa44e27b098")                                                \
    /* Instruments from the GM2 Analog Set */                                                                          \
    X(AnBass, "Analog Bass Drum", "edffed89-9966-49cb-af9d-d58653e2ebc0")                                              \
    X(ARmSht, "Analog Rim Shot", "694631a7-e092-4d65-a9f1-86c858c2a1b5")                                               \
    X(AnSnr1, "Analog Snare 1", "1c548dcc-57c3-4924-a054-51814320fb2e")                                                \
    X(ALwTm2, "Analog Low Tom 2", "990eefbc-7110-4b6e-8230-48325b0cd79d")                                              \
    X(ACHH1, "Analog CHH 1", "c30f2af5-053e-4169-b528-193823ff9e23")                                                   \
    X(ALwTm1, "Analog Low Tom 1", "0295a2f3-49f7-409f-ae7a-5191d740034c")                                              \
    X(ACHH2, "Analog CHH 2", "1f014668-0904-4307-8396-5805e21a2973")                                                   \
    X(AMdTm2, "Analog Mid Tom 2", "9b6af2ac-b5a6-47c3-94ef-ec5983d79f4d")                                              \
    X(AOHH, "Analog OHH", "135a0740-7f85-44d6-954f-5b04cc5134e0")                                                      \
    X(AMdTm1, "Analog Mid Tom 1", "3e763326-69e2-4865-8b47-897f3769d02e")                                              \
    X(AHiTm2, "Analog Hi Tom 2", "ad3b5e9d-eaca-4513-8ca8-9e0a06584a09")                                               \
    X(AnCym, "Analog Cymbal", "4b09f4ef-fbea-4a5e-816f-d4e09a16f29a")                                                  \
    X(AHiTm1, "Analog Hi Tom 1", "46e561fb-6017-4f67-be0e-ff5683c0b999")                                               \
    X(ACwbll, "Analog Cowbell", "c8b52f3e-b45e-48af-8874-613e4cd7936a")                                                \
    X(AHCnga, "Analog High Conga", "eadcb97d-767e-4c53-9b19-d86236211a9a")                                             \
    X(AMCnga, "Analog Mid Conga", "cc1e621a-7e3d-44cc-9b42-5820d2378e34")                                              \
    X(ALCnga, "Analog Low Conga", "3d35aaee-df50-43f5-8d3b-a42a305a3eb0")                                              \
    X(AMrcas, "Analog Maracas", "376758c3-bdcb-4fed-bfb9-d11389ffc23f")                                                \
    X(AClavs, "Analog Claves", "f4d5e1e7-f927-4021-8516-f883b4bc2222")                                                 \
    /* Instruments of the GM2 Jazz Set */                                                                              \
    X(JKick2, "Jazz Kick 2", "eb3984a9-134e-478f-abc1-b3f7853891c0")                                                   \
    X(JKick1, "Jazz Kick 1", "11d2a1ff-e98d-4e26-954b-6f8182d811cd")                                                   \
    /* Instruments of the GM2 Brush Set */                                                                             \
    X(BrTap, "Brush Tap", "7cf52502-5bac-43bc-9a83-6c3e62039732")                                                      \
    X(BrSlap, "Brush Slap", "c35c576c-6b02-46f3-94bd-d76af0330adf")                                                    \
    X(BrSwrL, "Brush Swirl", "ab6149aa-754f-43b4-acf6-2d6b8167ca0d")                                                   \
    /* Instruments of the GM2 Orchestra Set */                                                                         \
    X(ClHH2, "Closed Hi-hat 2", "07d0de4d-0857-451d-ad7f-5f53a3320ba3")                                                \
    X(OpHH2, "Open Hi-hat 2", "e3a28b92-3b8a-4767-8241-1d90018f1123")                                                  \
    X(CnBD2, "Concert BD 2", "83f949aa-80e5-4950-8441-c625e7a27ee8")                                                   \
    X(CnBD1, "Concert BD 1", "243ba465-60f4-4905-b7ee-afa08a5dca66")                                                   \
    X(CnSD, "Concert SD", "1d155d4c-22c4-4199-87a6-d5636acd2fc7")                                                      \
    X(TimpF, "Timpani F", "a21db818-b9c6-4924-991f-e49f80686d11")                                                      \
    X(TimpFs, "Timpani F#", "20052958-37d3-4644-8379-65e75ff9c4a0")                                                    \
    X(TimpG, "Timpani G", "741f70c2-99e0-4145-9390-643386c484ca")                                                      \
    X(TimpGs, "Timpani G#", "5c51f323-3c25-45b0-b963-79add3ec4aa9")                                                    \
    X(TimpA, "Timpani A", "dd2cfd58-4d4f-453c-b25f-5b8cfcb192e4")                                                      \
    X(TimpAs, "Timpani A#", "9d67af57-43b3-4729-9cc6-84e4c7056639")                                                    \
    X(TimpB, "Timpani B", "4522c95a-86db-4acc-ac5a-7634ce3d9d28")                                                      \
    X(Timpc, "Timpani c", "3810915d-1ff8-499d-be65-d931d5add550")                                                      \
    X(Timpcs, "Timpani c#", "f7512577-290c-4f09-bf72-9dea5a474f76")                                                    \
    X(Timpd, "Timpani d", "61e52c7d-73b3-43c9-ae9a-fea2aed1025f")                                                      \
    X(Timpds, "Timpani d#", "0dea9c66-bbd7-4cb9-9f4e-4f371eedb2b1")                                                    \
    X(Timpe, "Timpani e", "dacfdb72-a758-41ee-a076-63104b536788")                                                      \
    X(Timpf, "Timpani f", "0094a231-a4ec-49d5-b6b2-ec75cd95eeeb")                                                      \
    X(CnCym2, "Concert Cymbal 2", "ed3cb318-c5d1-44b5-808d-a7a3708883ec")                                              \
    X(CnCym1, "Concert Cymbal 1", "d1e439d2-a4aa-4155-8cca-932a18f6de53")                                              \
    X(Aplaus, "Applause", "9c0b6fa1-9e84-4321-9e5a-c192f02f04e8")                                                      \
    /* Instruments of the GM2 SFX Set*/                                                                                \
    X(GFret, "Guitar Fret Noise", "aa66eb2a-7a28-4e69-9c54-b82bec6e325c")                                              \
    X(GCutUp, "Guitar Cutting Noise Up", "ec817b1d-724c-42de-95dd-157d39ea2f88")                                       \
    X(GCutDn, "Guitar Cutting Noise Down", "73afc0e5-ada5-4612-9796-efaf5afc520e")                                     \
    X(DBsSlp, "String Slap of Double Bass", "183b24b0-be97-461e-9938-b6eac7af8305")                                    \
    X(FKyClk, "Fl.Key Click", "82e259d6-5b82-46a0-ae9d-af4a82bcdccc")                                                  \
    X(Laugh, "Laughing", "ecad883e-5fc6-4354-adf2-c04420d408a2")                                                       \
    X(Scream, "Scream", "72172cbf-a239-4348-90de-d8d3feb63883")                                                        \
    X(Punch, "Punch", "e0405dc7-a198-46c6-a941-17e890234020")                                                          \
    X(HeartB, "Heart Beat", "835db2bc-b537-4881-9569-e70cecd8513a")                                                    \
    X(Foot1, "Footsteps 1", "bd261e33-5393-46b5-80df-7f5ec9def0da")                                                    \
    X(Foot2, "Footsteps 2", "5c508ed1-1f97-4c9f-84ce-f23603298f4f")                                                    \
    X(DoorCr, "Door Creaking", "e5954f90-e55b-42c6-a892-ba704a3c8c4a")                                                 \
    X(Door, "Door", "f503579e-399d-4b0c-a3de-1b4ff499041d")                                                            \
    X(Scrtch, "Scratch", "23ca7688-b3a9-4d56-8223-863ff02fb76f")                                                       \
    X(WChime, "Wind Chimes", "0a5b3d9c-e549-4e52-a0b4-8c6263c3c68d")                                                   \
    X(CarEng, "Car-Engine", "c67aa407-232a-421e-b9dc-e51f97f82666")                                                    \
    X(CarStp, "Car-Stop", "f05bc125-f4cb-422d-9a81-a43299b1a5f0")                                                      \
    X(CarPss, "Car-Pass", "06d3bfc7-ccf0-4c79-ade5-9024d1067308")                                                      \
    X(CarCrs, "Car-Crash", "cb939c19-8e23-45a7-8cce-5df872e70fe1")                                                     \
    X(Siren, "Siren", "6206833b-5c89-4a77-aa0e-1d6c289ceb13")                                                          \
    X(Train, "Train", "5e99c16f-a06f-48a0-b530-b66b148918f9")                                                          \
    X(Jetpln, "Jetplane", "71b1d167-2ad9-4ad8-9613-c0448e815dd8")                                                      \
    X(Heli, "Helicopter", "5100a13e-4605-461e-9da8-ef757910d23c")                                                      \
    X(Strshp, "Starship", "040b1158-eeb3-4bf9-9541-f09e217c3344")                                                      \
    X(GunSht, "Gun Shot", "35f4a5af-66c8-4dfb-95ef-0fc18bd4a8a8")                                                      \
    X(MGun, "Machine Gun", "7ade1725-d91a-4a48-b521-81bc315c3f41")                                                     \
    X(Lsrgun, "Lasergun", "ce9abcc4-88d7-4fae-b19d-0e1dc3ed8855")                                                      \
    X(Explsn, "Explosion", "a5d401d5-55b6-422c-9b5e-bc3a8fd71007")                                                     \
    X(Dog, "Dog", "65df01c9-3cb0-4de9-989f-978b5f160d64")                                                              \
    X(Gallop, "Horse-Gallop", "8fe8ca8c-5018-4f21-8b70-3a04ad498123")                                                  \
    X(Birds, "Birds", "c0a50e5f-1f96-40e0-8fcd-28996605f4c9")                                                          \
    X(Rain, "Rain", "07017f06-4109-4bea-aa7a-cf25d298bcfa")                                                            \
    X(Thnder, "Thunder", "8337de9c-837a-4c4e-b894-529e16a424c5")                                                       \
    X(Wind, "Wind", "c6c86918-1618-4c1b-90a5-a8a0e8527df4")                                                            \
    X(Seashr, "Seashore", "802751a0-4cf3-4313-8e05-943432becc37")                                                      \
    X(Stream, "Stream", "70645217-c810-4009-a186-4113797c622f")                                                        \
    X(Bubble, "Bubble", "8b4b34a6-6cee-4777-85b6-a1568fe4c300")                                                        \
    /* Instruments of the GS standard 1 set */                                                                         \
    X(SnrRll, "Snare Roll", "d04bfd3c-9c76-49f7-8624-8e28bf48d70f")                                                    \
    X(FngSnp, "Finger Snap", "209836fb-3780-4efe-93d7-065fc3fc4403")                                                   \
    /* Instruments of the GS 808/909 set */                                                                            \
    X(Bs808, "808 Bass Drum", "82282139-14a5-4f33-9749-b517ab591ea4")                                                  \
    X(Bs909, "909 Bass Drum", "65edb0af-59e2-4543-ad5c-37f6d6f04f96")                                                  \
    /* Instruments of the XG standard 1 set */                                                                         \
    X(RimSht, "Open Rim Shot", "10744886-d7d9-4fae-bac3-1970490f01c8")                                                 \
    /* Giving these next two clicks slightly more generic identifiers */                                               \
    X(BeepLo, "Beep Low", "c7d34e2c-b7c9-440b-be57-e08819991337")  /* XG Seq Click L */                                \
    X(BeepHi, "Beep High", "0f1d756e-0b25-4bb4-b169-144c37becf05") /* XG Seq Click H */                                \
    X(BrSwrH, "Brush Swirl High", "9cc3f63b-05b5-4b38-a3ec-43b4a8337ca2")                                              \
    /* Instruments of the XG SFX 1 set */                                                                              \
    X(DCutNs, "Dist. Cut Noise", "67e1ad81-8200-47e7-89ee-0c8bfe2c7551")                                               \
    X(BssSld, "Bass Slide", "3e78e972-dd5e-4c16-9ee1-14b6ad8ab986")                                                    \
    X(PckScr, "Pick Scrape", "33cbb130-bcc4-4c22-b683-16a0778bcf47")                                                   \
    X(Kitty, "Kitty", "8c0bb60b-7cba-4785-acb8-2102be4d8f4a") /* Cat */                                                \
    X(Growl, "Growl", "0c15ca54-8b02-4294-a8bc-580fe4678c5d") /* Dog */                                                \
    X(Ghost, "Ghost", "e0255357-e611-41d5-9d53-15b93c0473c3")                                                          \
    /* Instruments of the XG SFX 2 set */                                                                              \
    X(DialTn, "Dial Tone", "c4de31a0-680b-4d86-8671-ee2a4e983e54")                                                     \
    X(TeleRn, "Telephone Ring", "40f514c0-5a46-4ec7-b3a1-18e238f04c47")                                                \
    X(TireSc, "Tire Screech", "3cb39aa0-67fe-448c-8861-cdadf0e0a372") /* Brake, Tyre */                                \
    X(Submrn, "Submarine", "f36acda3-c89d-4d1b-9e41-f6b62d83b9d0")                                                     \
    X(Frewrk, "FireWork", "0e2ca006-d30a-466d-abe6-71504102dfe3")

namespace bw_music {
    /// All percussion instruments built into BabelWires-Music.
    /// This is just a holder of instruments. It cannot be used as a PercussionSet because the values do not have a 1-1
    /// mapping to pitch values.
    class BuiltInPercussionInstruments : public babelwires::EnumType {
      public:
        REGISTERED_TYPE("BuiltInPerc", "Built In Percussion", "c67f1a9e-653d-42b5-bf73-1fdc2f8a9b1a", 1);

        BuiltInPercussionInstruments();

        ENUM_DEFINE_CPP_ENUM(BUILT_IN_PERCUSSION_INSTRUMENTS);
    };
} // namespace bw_music
