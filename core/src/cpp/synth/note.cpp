/**
 *   888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
 *   8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
 *   88888b.d88888 888    888 888          d8P 888  888  d8P
 *   888Y88888P888 888        888d888b.   d8P  888  888d88K
 *   888 Y888P 888 888        888P "Y88b d88   888  8888888b
 *   888  Y8P  888 888    888 888    888 8888888888 888  Y88b
 *   888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
 *   888       888  "Y8888P"   "Y8888P"        888  888    Y88b
 *
 *    - 64-bit 680x0-inspired Virtual Machine and assembler -
 */

#include <cmath>
#include <synth/note.hpp>

namespace MC64K::Synth::Audio {

#ifdef AUDIO_PITCH_APPROX
/**
 * Lookup table for twoToX() approximation.
 *
 * We store 256 entries that represent the mantissa bits of the 256 powers of 2.0 in the range
 * from 0.0 to 255.0/256.0. We also store a 257th entry which contains the largest mantissa
 * possible as an approximation for the result when going to the next highest exponent.
 */
constexpr uint32 const aPow2LUT[257] = {
    0x00000000, // 1.000000
    0x000058D8, // 1.002711
    0x0000B1ED, // 1.005430
    0x00010B41, // 1.008156
    0x000164D2, // 1.010889
    0x0001BEA1, // 1.013630
    0x000218AF, // 1.016378
    0x000272FC, // 1.019134
    0x0002CD87, // 1.021897
    0x00032850, // 1.024668
    0x00038359, // 1.027446
    0x0003DEA1, // 1.030232
    0x00043A29, // 1.033025
    0x000495F0, // 1.035826
    0x0004F1F6, // 1.038634
    0x00054E3D, // 1.041450
    0x0005AAC3, // 1.044274
    0x0006078A, // 1.047105
    0x00066491, // 1.049944
    0x0006C1D9, // 1.052791
    0x00071F62, // 1.055645
    0x00077D2B, // 1.058507
    0x0007DB35, // 1.061377
    0x00083981, // 1.064255
    0x0008980F, // 1.067140
    0x0008F6DD, // 1.070034
    0x000955EE, // 1.072935
    0x0009B541, // 1.075844
    0x000A14D5, // 1.078761
    0x000A74AD, // 1.081686
    0x000AD4C6, // 1.084618
    0x000B3523, // 1.087559
    0x000B95C2, // 1.090508
    0x000BF6A4, // 1.093464
    0x000C57CA, // 1.096429
    0x000CB933, // 1.099402
    0x000D1ADF, // 1.102383
    0x000D7CD0, // 1.105371
    0x000DDF04, // 1.108368
    0x000E417D, // 1.111374
    0x000EA43A, // 1.114387
    0x000F073B, // 1.117408
    0x000F6A81, // 1.120438
    0x000FCE0C, // 1.123476
    0x001031DC, // 1.126522
    0x001095F2, // 1.129576
    0x0010FA4D, // 1.132639
    0x00115EED, // 1.135709
    0x0011C3D3, // 1.138789
    0x00122900, // 1.141876
    0x00128E72, // 1.144972
    0x0012F42C, // 1.148077
    0x00135A2B, // 1.151189
    0x0013C072, // 1.154310
    0x001426FF, // 1.157440
    0x00148DD4, // 1.160578
    0x0014F4F0, // 1.163725
    0x00155C53, // 1.166880
    0x0015C3FF, // 1.170044
    0x00162BF2, // 1.173216
    0x0016942D, // 1.176397
    0x0016FCB1, // 1.179587
    0x0017657D, // 1.182785
    0x0017CE92, // 1.185992
    0x001837F0, // 1.189207
    0x0018A197, // 1.192431
    0x00190B88, // 1.195664
    0x001975C2, // 1.198906
    0x0019E046, // 1.202157
    0x001A4B13, // 1.205416
    0x001AB62B, // 1.208684
    0x001B218D, // 1.211961
    0x001B8D3A, // 1.215247
    0x001BF931, // 1.218542
    0x001C6573, // 1.221846
    0x001CD201, // 1.225159
    0x001D3EDA, // 1.228481
    0x001DABFE, // 1.231811
    0x001E196E, // 1.235151
    0x001E872A, // 1.238500
    0x001EF532, // 1.241858
    0x001F6387, // 1.245225
    0x001FD228, // 1.248601
    0x00204116, // 1.251986
    0x0020B051, // 1.255381
    0x00211FD9, // 1.258784
    0x00218FAF, // 1.262197
    0x0021FFD2, // 1.265620
    0x00227043, // 1.269051
    0x0022E102, // 1.272492
    0x0023520F, // 1.275942
    0x0023C36B, // 1.279401
    0x00243516, // 1.282870
    0x0024A70F, // 1.286348
    0x00251958, // 1.289836
    0x00258BEF, // 1.293333
    0x0025FED7, // 1.296840
    0x0026720E, // 1.300356
    0x0026E595, // 1.303881
    0x0027596C, // 1.307416
    0x0027CD94, // 1.310961
    0x0028420C, // 1.314516
    0x0028B6D5, // 1.318080
    0x00292BEF, // 1.321653
    0x0029A15B, // 1.325237
    0x002A1718, // 1.328830
    0x002A8D26, // 1.332433
    0x002B0387, // 1.336045
    0x002B7A3A, // 1.339668
    0x002BF13F, // 1.343300
    0x002C6897, // 1.346942
    0x002CE041, // 1.350594
    0x002D583F, // 1.354256
    0x002DD090, // 1.357927
    0x002E4934, // 1.361609
    0x002EC22D, // 1.365301
    0x002F3B79, // 1.369002
    0x002FB519, // 1.372714
    0x00302F0E, // 1.376436
    0x0030A957, // 1.380168
    0x003123F6, // 1.383910
    0x00319EE9, // 1.387662
    0x00321A32, // 1.391424
    0x003295D0, // 1.395197
    0x003311C4, // 1.398980
    0x00338E0E, // 1.402773
    0x00340AAF, // 1.406576
    0x003487A6, // 1.410390
    0x003504F3, // 1.414214
    0x00358298, // 1.418048
    0x00360094, // 1.421893
    0x00367EE7, // 1.425748
    0x0036FD92, // 1.429613
    0x00377C95, // 1.433489
    0x0037FBF0, // 1.437376
    0x00387BA3, // 1.441273
    0x0038FBAF, // 1.445181
    0x00397C14, // 1.449099
    0x0039FCD2, // 1.453028
    0x003A7DEA, // 1.456968
    0x003AFF5B, // 1.460918
    0x003B8126, // 1.464879
    0x003C034A, // 1.468850
    0x003C85CA, // 1.472833
    0x003D08A4, // 1.476826
    0x003D8BD8, // 1.480830
    0x003E0F68, // 1.484845
    0x003E9353, // 1.488871
    0x003F179A, // 1.492908
    0x003F9C3C, // 1.496955
    0x0040213B, // 1.501014
    0x0040A695, // 1.505084
    0x00412C4D, // 1.509164
    0x0041B261, // 1.513256
    0x004238D2, // 1.517359
    0x0042BFA1, // 1.521473
    0x004346CD, // 1.525598
    0x0043CE57, // 1.529734
    0x0044563F, // 1.533882
    0x0044DE85, // 1.538041
    0x0045672A, // 1.542211
    0x0045F02E, // 1.546392
    0x00467991, // 1.550585
    0x00470353, // 1.554789
    0x00478D75, // 1.559004
    0x004817F7, // 1.563231
    0x0048A2D8, // 1.567470
    0x00492E1B, // 1.571720
    0x0049B9BE, // 1.575981
    0x004A45C1, // 1.580254
    0x004AD226, // 1.584538
    0x004B5EED, // 1.588834
    0x004BEC15, // 1.593142
    0x004C799F, // 1.597462
    0x004D078C, // 1.601793
    0x004D95DA, // 1.606136
    0x004E248C, // 1.610490
    0x004EB3A1, // 1.614857
    0x004F4319, // 1.619235
    0x004FD2F4, // 1.623625
    0x00506334, // 1.628027
    0x0050F3D7, // 1.632441
    0x005184DF, // 1.636867
    0x0052164C, // 1.641305
    0x0052A81E, // 1.645756
    0x00533A54, // 1.650218
    0x0053CCF1, // 1.654692
    0x00545FF3, // 1.659178
    0x0054F35B, // 1.663677
    0x00558729, // 1.668187
    0x00561B5E, // 1.672710
    0x0056AFFA, // 1.677245
    0x005744FD, // 1.681793
    0x0057DA67, // 1.686353
    0x00587039, // 1.690925
    0x00590673, // 1.695509
    0x00599D16, // 1.700106
    0x005A3421, // 1.704716
    0x005ACB94, // 1.709338
    0x005B6371, // 1.713972
    0x005BFBB8, // 1.718619
    0x005C9468, // 1.723279
    0x005D2D82, // 1.727951
    0x005DC706, // 1.732636
    0x005E60F5, // 1.737334
    0x005EFB4E, // 1.742044
    0x005F9613, // 1.746767
    0x00603143, // 1.751503
    0x0060CCDF, // 1.756252
    0x006168E7, // 1.761014
    0x0062055B, // 1.765788
    0x0062A23C, // 1.770576
    0x00633F89, // 1.775376
    0x0063DD44, // 1.780190
    0x00647B6D, // 1.785017
    0x00651A03, // 1.789856
    0x0065B907, // 1.794709
    0x00665879, // 1.799575
    0x0066F85B, // 1.804454
    0x006798AB, // 1.809347
    0x0068396A, // 1.814252
    0x0068DA99, // 1.819171
    0x00697C38, // 1.824103
    0x006A1E47, // 1.829049
    0x006AC0C7, // 1.834008
    0x006B63B7, // 1.838981
    0x006C0719, // 1.843967
    0x006CAAEC, // 1.848966
    0x006D4F30, // 1.853979
    0x006DF3E7, // 1.859006
    0x006E9910, // 1.864046
    0x006F3EAB, // 1.869100
    0x006FE4BA, // 1.874168
    0x00708B3B, // 1.879249
    0x00713231, // 1.884344
    0x0071D99A, // 1.889453
    0x00728177, // 1.894576
    0x007329C9, // 1.899713
    0x0073D290, // 1.904863
    0x00747BCC, // 1.910028
    0x0075257D, // 1.915207
    0x0075CFA4, // 1.920399
    0x00767A41, // 1.925606
    0x00772555, // 1.930827
    0x0077D0DF, // 1.936062
    0x00787CE1, // 1.941311
    0x0079295A, // 1.946574
    0x0079D64A, // 1.951852
    0x007A83B3, // 1.957144
    0x007B3194, // 1.962451
    0x007BDFED, // 1.967771
    0x007C8EC0, // 1.973106
    0x007D3E0C, // 1.978456
    0x007DEDD2, // 1.983820
    0x007E9E11, // 1.989199
    0x007F4ECB, // 1.994592
    0x007FFFFF, // 1.999999 // largest value stricly less than 2.0
};

/**
 * Approximates the calculation 2^x for floating point values of X. This is used to calculate the
 * frequency multiplier from fractional octave inputs when applying pitch shifts. This replaces
 * the use of std::pow(2.0, x) for this purpose which is a significant performance bottleneck.
 *
 * The approximation first calculates the floor(x) and uses this to determine the exponent value
 * we will return. Then, the difference between x and floor(x) (range 0.0 - 0.9r) is multiplied
 * by 256 to determine a lookup into the above table. The remainging fractional component of that
 * multiplication is finally used as an interpolating factor between the lookup value and the
 * adjacent lookup value that is one above. This is why the table is 257 entries long.
 *
 * The resulting linear interpolation of the two values gives a very good estimation for our
 * purposes.
 */
float32 twoToX(float32 fPow) {
    union {
        uint32 u32;
        float32 f32;
    };

    // Calculate the exponent of our result. This is based on the integer part of the
    // input.
    float32 fLower = std::floor(fPow);
    fPow -= fLower;

    // IEEE-754 float32 uses a biased 8-bit exponent (add 127) in bits 23:30
    uint32 uExponent = ((127 + (int)fLower) & 0xFF) << 23;  // exponent

    // Choose the closest index in the table
    fPow *= 256.0f;
    uint32 uIndex = (unsigned)fPow;

    // The remaining fractional part of fPow can be used as an interpolation factor
    // between two samples from the table for a better estimate.
    fPow   -= (float32)uIndex;

    u32 = uExponent | aPow2LUT[uIndex];     // closest mantissa below
    float32 fEstimate1 = f32;
    u32 = uExponent | aPow2LUT[uIndex + 1]; // closest mantissa above
    float32 fEstimate2 = f32;

    // Interpolate
    return (fEstimate2 * fPow) + (fEstimate1 * (1.0f - fPow));
}

float32 Note::semisToMultiplier(float32 fSemitones) {
    return twoToX(fSemitones * FACTOR_PER_SEMI);
}

#endif

float32 Note::getFrequency(
    int32   iNumber,
    float32 fBendSemis,
    float32 fScalePerOctave,
    float32 fCentreValue
) {
    float32 fNote  = fScalePerOctave * ((float32)(iNumber - CENTRE_REFERENCE) + fBendSemis);
    return fCentreValue * semisToMultiplier(fNote);
}

}
