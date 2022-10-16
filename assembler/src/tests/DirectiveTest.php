<?php

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

declare(strict_types = 1);

namespace ABadCafe\MC64K\Tests;
use ABadCafe\MC64K\Utils\TestCase;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\IO;
use ABadCafe\MC64K\Parser\SourceLine\Directive\Processor;

use function \sprintf;

/**
 * DirectiveTest
 *
 */
class DirectiveTest extends TestCase {

    const ALIGN_TEST_CASES = [
        ' @align 2, 2'  => "\x01\x02",
        ' @align 3, 3'  => "\x01\x02\x03",
        ' @align 4, 4'  => "\x01\x02\x03\x04",
        ' @align 5, 2'  => "\x01\x02\x03\x04",
        ' @align 6, 3'  => "\x01\x02\x03\x04\x06\x06",
        ' @align 8, 8'  => "\x01\x02\x03\x04\x06\x06\x08\x08",
        ' @align 1, 10' => "\x01\x02\x03\x04\x06\x06\x08\x08\x01\x01",
    ];

    const IMPORT_DIRECTIVE           = '@import';
    const IMPORT_TEST_NAME_TEMPLATE  = "imp_%s";
    const EXPORT_DIRECTIVE           = '@export';
    const EXPORT_TEST_NAME_TEMPLATE  = "exp_%s";
    const IMPORT_EXPORT_TEST_CASES = [
        'r'   => State\LabelLocation::IE_READ,
        'w'   => State\LabelLocation::IE_WRITE,
        'x'   => State\LabelLocation::IE_CALL,
        'rw'  => State\LabelLocation::IE_WRITE|State\LabelLocation::IE_READ,
        'wr'  => State\LabelLocation::IE_WRITE|State\LabelLocation::IE_READ,
        'rx'  => State\LabelLocation::IE_CALL|State\LabelLocation::IE_READ,
        'xr'  => State\LabelLocation::IE_CALL|State\LabelLocation::IE_READ,
        'wx'  => State\LabelLocation::IE_CALL|State\LabelLocation::IE_WRITE,
        'xw'  => State\LabelLocation::IE_CALL|State\LabelLocation::IE_WRITE,
        'rwx' => State\LabelLocation::IE_CALL|State\LabelLocation::IE_WRITE|State\LabelLocation::IE_READ,
        'wrx' => State\LabelLocation::IE_CALL|State\LabelLocation::IE_WRITE|State\LabelLocation::IE_READ,
        'rxw' => State\LabelLocation::IE_CALL|State\LabelLocation::IE_WRITE|State\LabelLocation::IE_READ,
        'xrw' => State\LabelLocation::IE_CALL|State\LabelLocation::IE_WRITE|State\LabelLocation::IE_READ,
        'xwr' => State\LabelLocation::IE_CALL|State\LabelLocation::IE_WRITE|State\LabelLocation::IE_READ,
    ];


    /**
     * @inheritDoc
     */
    public function run(): void {
        $this->testAlign();
        $this->testFlag();
        $this->testDefine();
        $this->testDuplicateDefineInFileThrows();
        $this->testDefineHasFileScope();
        $this->testEqu();
        $this->testEquHasGlobalScope();
        $this->testExport();
        $this->testImport();
        $this->testStackSize();
    }

    /**
     * Tests the align directive. This starts by creating a 1-byte misaligned output buffer and then applying successive
     * alignment padding to it, up to 10 bytes. Each alignment uses a different padding character so that we can
     * directly assert what the output buffer should look like after each test.
     */
    private function testAlign(): void {
        if ($this->isVerbose()) {
            echo "\ttesting @align\n";
        }
        $oBuffer = State\Coordinator::get()->getOutput();
        $this->assertSame(0, $oBuffer->getCurrentOffset());
        $oBuffer->appendStatement("\x01");
        $this->assertSame(1, $oBuffer->getCurrentOffset());

        $oProcessor = new Processor\Align();
        foreach (self::ALIGN_TEST_CASES as $sTestCase => $sExpect) {
            $oProcessor->process($sTestCase);
            $this->assertSame($sExpect, $oBuffer->getBytecode());
        }
    }

    /**
     * Tests the enable/disable (and abbreviated) directives. We first assert that a given pair of test flags are not
     * set. We then enable them, assert they have been set, disable them and assert they are cleared.
     */
    private function testFlag(): void {
        if ($this->isVerbose()) {
            echo "\ttesting @en/@enable\n";
        }
        $oOptions = State\Coordinator::get()->getOptions();
        $this->assertFalse($oOptions->isEnabled('unit_test_flag'));
        $this->assertFalse($oOptions->isEnabled('unit_test_flag_short'));

        $oProcessor = new Processor\Flag();
        $oProcessor->process(' @enable unit_test_flag');
        $oProcessor->process(' @en unit_test_flag_short');

        $this->assertTrue($oOptions->isEnabled('unit_test_flag'));
        $this->assertTrue($oOptions->isEnabled('unit_test_flag_short'));
        if ($this->isVerbose()) {
            echo "\ttesting @dis/@disable\n";
        }
        $oProcessor->process(' @disable unit_test_flag');
        $oProcessor->process(' @dis unit_test_flag_short');
        $this->assertFalse($oOptions->isEnabled('unit_test_flag'));
        $this->assertFalse($oOptions->isEnabled('unit_test_flag_short'));
    }


    /**
     * Tests the define/undefine (and abbreviated) directives. We first assert that a given pair of definitions are not
     * already defined. We then define them, assert they are defined and match the given definition. We then undefine
     * them and assert that they are no longer defined.
     */
    private function testDefine(): void {
        if ($this->isVerbose()) {
            echo "\ttesting @def/@define\n";
        }
        $oDefine = new Processor\Define();

        State\Coordinator::get()->setCurrentFile(new IO\SourceString('', __METHOD__));

        $oDefinitions = State\Coordinator::get()->getDefinitionSet();

        $aDefinitions = $oDefinitions->getDefinitions();
        $this->assertTrue(is_array($aDefinitions));
        $this->assertFalse(isset($aDefinitions['TEST1']));
        $this->assertFalse(isset($aDefinitions['TEST2']));

        $oDefine->process(' @define TEST1 test_1_replacement');
        $oDefine->process(' @def    TEST2 test_2_replacement');

        $aDefinitions = $oDefinitions->getDefinitions();
        $this->assertTrue(is_array($aDefinitions));
        $this->assertTrue(isset($aDefinitions['TEST1']));
        $this->assertTrue(isset($aDefinitions['TEST2']));
        $this->assertSame('test_1_replacement', $aDefinitions['TEST1']);
        $this->assertSame('test_2_replacement', $aDefinitions['TEST2']);

        if ($this->isVerbose()) {
            echo "\ttesting @undef\@undefine\n";
        }

        $oUndefine = new Processor\Undefine();
        $oUndefine->process(' @undef    TEST1');
        $oUndefine->process(' @undefine TEST2');
        $aDefinitions = $oDefinitions->getDefinitions();
        $this->assertTrue(is_array($aDefinitions));
        $this->assertFalse(isset($aDefinitions['TEST1']));
        $this->assertFalse(isset($aDefinitions['TEST2']));
    }

    /**
     * Asserts that duplicating a define that is already active in the current file is an error.
     */
    private function testDuplicateDefineInFileThrows(): void {
        if ($this->isVerbose()) {
            echo "\ttesting duplicate define throws\n";
        }

        $oDefine = new Processor\Define();

        State\Coordinator::get()->setCurrentFile(new IO\SourceString('', __METHOD__));
        $oDefinitions = State\Coordinator::get()->getDefinitionSet();

        // Make sure the definition is not already set up
        $aDefinitions = $oDefinitions->getDefinitions();
        $this->assertTrue(is_array($aDefinitions));
        $this->assertFalse(isset($aDefinitions['DUPE_1']));

        // First define should be fine
        $oDefine->process(' @define DUPE_1 123');
        $aDefinitions = $oDefinitions->getDefinitions();
        $this->assertTrue(is_array($aDefinitions));
        $this->assertTrue(isset($aDefinitions['DUPE_1']));
        $this->assertSame('123', $aDefinitions['DUPE_1']);

        // A second attempt should throw
        try {
            $oDefine->process(' @define DUPE_1 456');
            $this->assertTrue(false);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\RuntimeException::class, $oError);
        }
    }

    /**
     * Asserts that duplicating a define in a different file is not an error since defines have file scope.
     */
    private function testDefineHasFileScope(): void {
        if ($this->isVerbose()) {
            echo "\ttesting define has file scope\n";
        }

        $oDefine = new Processor\Define();

        // Change the file
        State\Coordinator::get()->setCurrentFile(new IO\SourceString('', __METHOD__ . '1'));
        $oDefinitions = State\Coordinator::get()->getDefinitionSet();

        // Make sure the definition is not already set up
        $aDefinitions = $oDefinitions->getDefinitions();

        $this->assertTrue(is_array($aDefinitions));
        $this->assertFalse(isset($aDefinitions['DUPE_1']));

        // First define should be fine
        $oDefine->process(' @define DUPE_1 123');
        $aDefinitions = $oDefinitions->getDefinitions();
        $this->assertTrue(is_array($aDefinitions));
        $this->assertTrue(isset($aDefinitions['DUPE_1']));
        $this->assertSame('123', $aDefinitions['DUPE_1']);

        // Change the file
        State\Coordinator::get()->setCurrentFile(new IO\SourceString('', __METHOD__ . '2'));

        // A second attempt should not throw
        $oDefine->process(' @define DUPE_1 456');
    }

    /**
     * Asserts that the equ directive sets a definition
     */
    private function testEqu(): void {
        if ($this->isVerbose()) {
            echo "\ttesting @equ\n";
        }
        $oDefine = new Processor\Define();

        State\Coordinator::get()->setCurrentFile(new IO\SourceString('', __METHOD__));

        $oDefinitions = State\Coordinator::get()->getDefinitionSet();

        $aDefinitions = $oDefinitions->getDefinitions();
        $this->assertTrue(is_array($aDefinitions));
        $this->assertFalse(isset($aDefinitions['TEST_EQU']));

        // First define should be fine
        $oDefine->process(' @equ TEST_EQU 678');

        // EQU should be visible in file scope
        $aDefinitions = $oDefinitions->getDefinitions();
        $this->assertTrue(is_array($aDefinitions));
        $this->assertTrue(isset($aDefinitions['TEST_EQU']));
        $this->assertSame('678', $aDefinitions['TEST_EQU']);
    }

    /**
     * Asserts that a set equ definition persists when file changes.
     */
    private function testEquHasGlobalScope(): void {
        if ($this->isVerbose()) {
            echo "\ttesting @equ has global scope\n";
        }

        $oDefine = new Processor\Define();

        State\Coordinator::get()->setCurrentFile(new IO\SourceString('', __METHOD__. '1'));

        $oDefinitions = State\Coordinator::get()->getDefinitionSet();

        $aDefinitions = $oDefinitions->getDefinitions();
        $this->assertTrue(is_array($aDefinitions));
        $this->assertFalse(isset($aDefinitions['TEST_EQU_GLOBAL']));

        // First define should be fine
        $oDefine->process(' @equ TEST_EQU_GLOBAL ABC');

        // EQU should be visible in file scope
        $aDefinitions = $oDefinitions->getDefinitions();
        $this->assertTrue(is_array($aDefinitions));
        $this->assertTrue(isset($aDefinitions['TEST_EQU_GLOBAL']));
        $this->assertSame('ABC', $aDefinitions['TEST_EQU_GLOBAL']);

        // Change the file
        State\Coordinator::get()->setCurrentFile(new IO\SourceString('', __METHOD__ . '2'));

        // EQU should still be visible in file scope
        $aDefinitions = $oDefinitions->getDefinitions();
        $this->assertTrue(is_array($aDefinitions));
        $this->assertTrue(isset($aDefinitions['TEST_EQU_GLOBAL']));
        $this->assertSame('ABC', $aDefinitions['TEST_EQU_GLOBAL']);
    }

    /**
     * Tests the export directive. We declare a set of export labels with each of the the read/write/exec attribute
     * combinations set in IMPORT_EXPORT_TEST_CASES. For each one, we first assert it is not in the exported set. Then
     * we add it and confirm it has been added with the correct attributes.
     */
    private function testExport(): void {
        if ($this->isVerbose()) {
            echo "\ttesting ", self::EXPORT_DIRECTIVE, "\n";
        }

        $oExport = new Processor\Export();

        $oLabelLocation = State\Coordinator::get()
            ->getLabelLocation();

        foreach (self::IMPORT_EXPORT_TEST_CASES as $sQualification => $iExpect) {
            $sTestCase   = sprintf(self::EXPORT_TEST_NAME_TEMPLATE, $sQualification);

            $aImportExports = $oLabelLocation->getImportExportQualifications();
            $this->assertTrue(is_array($aImportExports));
            $this->assertFalse(isset($aImportExports[$sTestCase]));

            $sTestString = sprintf(
                " %s %s %s\n",
                self::EXPORT_DIRECTIVE,
                $sTestCase,
                $sQualification
            );
            $oExport->process($sTestString);
            $aImportExports = $oLabelLocation->getImportExportQualifications();
            $this->assertTrue(is_array($aImportExports));
            $this->assertTrue(isset($aImportExports[$sTestCase]));
            $this->assertSame($iExpect, $aImportExports[$sTestCase]);
        }
    }

    /**
     * Tests the import directive. We declare a set of export labels with each of the the read/write/exec attribute
     * combinations set in IMPORT_EXPORT_TEST_CASES. For each one, we first assert it is not in the imported set. Then
     * we add it and confirm it has been added with the correct attributes.
     */
    private function testImport(): void {
        if ($this->isVerbose()) {
            echo "\ttesting ", self::IMPORT_DIRECTIVE, "\n";
        }

        $oImport = new Processor\Import();

        $oLabelLocation = State\Coordinator::get()
            ->getLabelLocation();
        foreach (self::IMPORT_EXPORT_TEST_CASES as $sQualification => $iExpect) {
            $sTestCase   = sprintf(self::IMPORT_TEST_NAME_TEMPLATE, $sQualification);

            $aImportExports = $oLabelLocation->getImportExportQualifications();
            $this->assertTrue(is_array($aImportExports));
            $this->assertFalse(isset($aImportExports[$sTestCase]));

            $sTestString = sprintf(
                " %s %s %s\n",
                self::IMPORT_DIRECTIVE,
                $sTestCase,
                $sQualification
            );
            $oImport->process($sTestString);
            $aImportExports = $oLabelLocation->getImportExportQualifications();
            $this->assertTrue(is_array($aImportExports));
            $this->assertTrue(isset($aImportExports[$sTestCase]));
            $this->assertSame($iExpect, $aImportExports[$sTestCase]);
        }
    }

    /**
     * Tests the behaviour of the stacksize directive. At present it is only legal to make the stack larger than
     * than it was. Trying to shrink it will not raise an error but is ignored.
     */
    private function testStackSize(): void {
        if ($this->isVerbose()) {
            echo "\ttesting @stacksize\n";
        }
        $oStackSize = new Processor\StackSize();
        $oOptions   = State\Coordinator::get()
            ->getGlobalOptions();
        $iInitialStackSize = $oOptions->getInt(Defs\Project\IOptions::APP_STACK_SIZE, -1);
        $this->assertTrue($iInitialStackSize > 1);

        // Asking for a smaller stack than is already set should be rejected.
        $iHalfStackSize = $iInitialStackSize >> 1;
        $oStackSize->process(" @stacksize " . $iHalfStackSize);
        $this->assertSame($iInitialStackSize, $oOptions->getInt(Defs\Project\IOptions::APP_STACK_SIZE, -1));

        // Asking for a larger stack than is already set should be accepted.
        $iTwiceStackSize = $iInitialStackSize << 1;
        $oStackSize->process(" @stacksize " . $iTwiceStackSize);
        $this->assertSame($iTwiceStackSize, $oOptions->getInt(Defs\Project\IOptions::APP_STACK_SIZE, -1));

    }
}
