# Third-Party Notices

Zombie Siege's own source code and original content are licensed under the MIT licence (see [LICENSE.md](LICENSE.md)).
The third-party software and assets listed below are **not** covered by that licence. They remain the property of
their respective owners and are used under their own terms.

Assets marked **private** are not included in this repository. They live in the private
`ZombieSiegeAssets` repo, which is cloned into `Content/ExternalAssets` (see the README).

## Software

| Name | Owner | Licence | Location |
|---|---|---|---|
| Unreal Engine 5.7 | Epic Games | [Unreal Engine EULA](https://www.unrealengine.com/eula) | Not included |
| FMOD Studio / FMOD for Unreal 2.03.15 (incl. FMODStudioNiagara) | Firelight Technologies Pty Ltd | [FMOD EULA](https://www.fmod.com/legal) | `Plugins/FMODStudio`, `Plugins/FMODStudioNiagara` |

Audio engine: **FMOD Studio by Firelight Technologies Pty Ltd.**

## Art, animation and fonts

| Name | Owner / source | Licence | Location |
|---|---|---|---|
| POLYGON City Zombies, POLYGON Military, POLYGON Battle Royale | Synty Studios | Synty Store licence | Private (`ExternalAssets/Polygon*`, `SKEL_Synty`) |
| Animation Starter Pack | Epic Games (Fab) | Fab / Epic Content licence | Private (`ExternalAssets/AnimStarterPack`) |
| Population Zero BB font | Blambot, via FontSpace | Free licence covers comic use only, **not video games**. **To be replaced** before any release. | Private (`ExternalAssets/Fonts/PopulationZeroBB`) |
| Mixamo animations (retargeted to the Synty skeleton) | Adobe Mixamo | [Adobe Mixamo terms](https://helpx.adobe.com/creative-cloud/faq/mixamo-faq.html) | `Content/Animations/Synty` |

## Sound effects

The raw source audio is **private** (`ExternalAssets/FMODAudio`). The built FMOD banks in `Content/FMOD/Desktop`
contain these sounds in packaged form. Paths below are relative to `FMODAudio/`.

**Sources and licences**
- **Pixabay**: [Pixabay Content License](https://pixabay.com/service/license-summary/)
- **Sonniss GDC Game Audio Bundles**: [Sonniss GDC bundle licence](https://sonniss.com/gameaudiogdc/)
- **SoundDino**: free licence, attribution not required ([sounddino.com](https://sounddino.com/)). Credited here voluntarily.

### Ambiance
| File | Source |
|---|---|
| `Ambiance/spooky-wind-57845.mp3` | Pixabay: https://pixabay.com/sound-effects/spooky-wind-57845/ |
| `Ambiance/dark-wind-19785.mp3` | Pixabay: https://pixabay.com/sound-effects/dark-wind-19785/ |
| `Ambiance/Background Gunfire/battle-war-zone-gun-fight-32155*.{mp3,wav}` | Pixabay: https://pixabay.com/sound-effects/battle-war-zone-gun-fight-32155/ (trimmed copies) |
| `Explosions/medium-explosion-40472.mp3` | Pixabay: https://pixabay.com/sound-effects/medium-explosion-40472/ |

### Weapons
| File | Source |
|---|---|
| `Weapons/Gunfire/single-pistol-gunshot-33-37187.mp3` | Pixabay: https://pixabay.com/sound-effects/single-pistol-gunshot-33-37187/ |
| `Weapons/Gunfire/m16-rifle-shots-105797.mp3` | Pixabay: https://pixabay.com/sound-effects/m16-rifle-shots-105797/ |
| `Weapons/Gunfire/Glock17/Glock 17 - FIRING - Take 1 - 20m In Front - AB - MKH8020.wav` (and the `Glock17 - 1..9` edits) | Sonniss GDC 2023: Pole Position |
| `Weapons/Reloads/mag-reload-81594.mp3` | Pixabay: https://pixabay.com/sound-effects/mag-reload-81594/ |
| `Weapons/Empty/rifle-clip-empty-98832.mp3` | Pixabay: https://pixabay.com/sound-effects/rifle-clip-empty-98832/ |

### Zombie
| File | Source |
|---|---|
| `Zombie/zombie-death-2-95167.mp3` | Pixabay: https://pixabay.com/sound-effects/zombie-death-2-95167/ |
| `Zombie/monster-death-grunt-131480.mp3` | Pixabay: https://pixabay.com/sound-effects/monster-death-grunt-131480/ |
| `Zombie/zombie-attack-6419.mp3` | Pixabay: https://pixabay.com/sound-effects/zombie-attack-6419/ |
| `Zombie/Attacks/zombie-growls-and-roars-filter-effect*.{mp3,wav}` | SoundDino: "Zombie growls and roars (filter effect)", https://sounddino.com/en/effects/zombies/ (trimmed copies) |

### Player
| File | Source |
|---|---|
| `Player/male_hurt7-48124.mp3` | Pixabay: https://pixabay.com/sound-effects/male-hurt7-48124/ |
| `Player/male-death-sound-128357.mp3` | Pixabay: https://pixabay.com/sound-effects/male-death-sound-128357/ |
| `Player/heartbeat-sound-effects-for-you-122458.mp3` | Pixabay: https://pixabay.com/sound-effects/heartbeat-sound-effects-for-you-122458/ |

### Money
| File | Source |
|---|---|
| `Money/cash-register-purchase-87313.mp3` | Pixabay: https://pixabay.com/sound-effects/cash-register-purchase-87313/ |
| `Money/coins-falling-013-36967.mp3` | Pixabay: https://pixabay.com/sound-effects/coins-falling-013-36967/ |

### Footsteps
| File | Source |
|---|---|
| `Footsteps/Concrete/concrete-footsteps-1-6265.mp3` (and the `concrete-footsteps-1..9.wav` edits) | Pixabay: https://pixabay.com/sound-effects/concrete-footsteps-1-6265/ |

### Barricades
| File | Source |
|---|---|
| `Barricade/Wooden Tearing 09.wav` | Sonniss GDC 2017: Double Trouble Audio, Tools and Wood |
| `Barricade/Small Wood Pile, Smash.wav` | Sonniss GDC 2017: Double Trouble Audio, Tools and Wood |
| `Barricade/WOODCrsh_Designed Wood Crash And Debris 13_DDUMAIS_NONE.wav` | Sonniss GDC 2024: David Dumais, Explosion SFX Pack |
| `Barricade/WOODBrk_Snap09_InMotionAudio_Wood*.wav` | Sonniss GDC bundle: InMotion Audio, Wood (*year to confirm*) |

### Previously listed, not currently in the FMOD project
These were in the old `SoundEffectsSource.txt`, but no matching file exists in the FMOD project:
- `tubular-bell-of-death-89485`, Pixabay: https://pixabay.com/sound-effects/tubular-bell-of-death-89485/ (game over)
- `BROKEN - CK - BEAM Wood Ceiling Drop On Concrete`, Sonniss GDC 2019: Rock The Speakerbox, Broken
