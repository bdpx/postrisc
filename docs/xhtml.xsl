<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
  xmlns="http://www.w3.org/1999/xhtml"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:msxsl="urn:schemas-microsoft-com:xslt"
  xmlns:exslt="http://exslt.org/common"
  xmlns:str="http://exslt.org/strings"
  extension-element-prefixes="msxsl exslt str">

<!--
  xmlns:xhtml="http://www.w3.org/1999/xhtml"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:fo="http://www.w3.org/1999/Format"
  xmlns:xi="http://www.w3.org/2001/XInclude"
  method="html"
  version="4.0"
  version="1.1"
  doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN"
  doctype-system="http://www.w3.org/TR/html4/loose.dtd"

  doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"
  doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"

-->

<xsl:output
  media-type="text/html"
  method="xml"
  encoding="utf-8"
  indent="yes"
  omit-xml-declaration="no"
  doctype-public="-//W3C//DTD XHTML 1.1//EN"
  doctype-system="http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd"
/>

<!-- preserve -->
<xsl:preserve-space elements="*"/>
<xsl:preserve-space elements="p"/>
<xsl:preserve-space elements="td"/>
<xsl:preserve-space elements="li"/>
<xsl:preserve-space elements="note"/>


<!--
first stage tags
-->

<xsl:template match="myinclude">
 <xsl:copy-of select="document(text())"/>
</xsl:template>

<xsl:template match="include">
<!--
 <xsl:apply-templates select="document(@href)"/>
-->
  <p><a>
  <xsl:attribute name="href">
     <xsl:value-of select="@href"/>
  </xsl:attribute>
  <xsl:value-of select="document(@href)/chapter/title"/>
  </a></p>
</xsl:template>

<xsl:template match="prebook">
  <!-- preload all includes into root non-xhtml 'book' tag without default namespace -->
  <xsl:variable name="first">
    <xsl:element name="book" namespace="">
      <xsl:apply-templates/>
    </xsl:element>
  </xsl:variable>
  <!-- parse included data, node-set problem -->
  <xsl:choose>
    <xsl:when test="function-available('msxsl:node-set')">
      <xsl:apply-templates select="msxsl:node-set($first)"/>
    </xsl:when>
    <xsl:when test="function-available('exslt:node-set')">
      <xsl:apply-templates select="exslt:node-set($first)"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:apply-templates select="$first"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="prebook/title">
  <title>
    <xsl:apply-templates/>
  </title>
</xsl:template>

<xsl:template match="precontent">
<content/>
</xsl:template>

<!-- 
second stage tags
-->

<xsl:template match="archname">POSTRISC</xsl:template>

<xsl:template name="footer">
  <div class="chapter">
  <br/><br/><hr/>
  <p class="copyright">POSTRISC 64-bit virtual processor.<br/>
Instruction Set Architecture (ISA) and assembler syntax.<br/>
Copyright &#169; 2003-2022 by Dmitry Buvaylo.</p>
  </div>
</xsl:template>

<xsl:template name="chapter_content">
  <xsl:if test="not(parent::book)">
    <h2>Content</h2>
  </xsl:if>
  <xsl:for-each select="section">
    <p class="toc2"><xsl:text>&#167; </xsl:text><xsl:number count="chapter|section" level="multiple" format="1.1. "/>
    <a>
    <xsl:attribute name="href">
      <xsl:value-of select="@href"/>#<xsl:value-of select="@id"/>
    </xsl:attribute>
    <xsl:for-each select="title">
      <xsl:apply-templates/>
    </xsl:for-each>
    </a></p>
    <xsl:for-each select="subsection">
      <p class="toc3"><xsl:text>&#167; </xsl:text><xsl:number count="chapter|section|subsection" level="multiple" format="1.1.1. "/>
      <a>
      <xsl:attribute name="href">
        <xsl:value-of select="@href"/>#<xsl:value-of select="@id"/>
      </xsl:attribute>
      <xsl:for-each select="title">
        <xsl:apply-templates/>
      </xsl:for-each>
      </a></p>
    </xsl:for-each>
  </xsl:for-each>
</xsl:template>

<xsl:template name="book_content">
  <div class="chapter">
  <h1>Content</h1>
  <xsl:for-each select="chapter">
    <p class="toc1"><xsl:text>Chapter </xsl:text><xsl:number count="chapter" format="1. "/>
    <a>
    <xsl:attribute name="href">
      <xsl:value-of select="@href"/>#<xsl:value-of select="@id"/>
    </xsl:attribute>
    <xsl:for-each select="title">
      <xsl:apply-templates/>
    </xsl:for-each>
    </a></p>
    <xsl:call-template name="chapter_content"/>
  </xsl:for-each>
  </div>
</xsl:template>

<xsl:template name="page_head">
  <head>
  <title>
    <xsl:for-each select="./preface/title">
      <xsl:apply-templates/>
    </xsl:for-each>
  </title>
  <meta http-equiv="content-type" content="text/html; charset=utf-8"/>
  <meta http-equiv="content-language" content="en"/>
  <meta http-equiv="Content-Style-Type" content="text/css"/>
  <meta name="viewport" content="width=device-width"/>
  <meta name="language" content="en"/>
  <link href="styles.css" rel="stylesheet" type="text/css"/>
  </head>
</xsl:template>

<xsl:template match="book">
  <html>
  <xsl:call-template name="page_head"/>
  <body>
  <xsl:apply-templates select="./preface"/>
  <xsl:call-template name="book_content"/>
  <xsl:for-each select="./chapter">
     <div class="chapter">
       <xsl:apply-templates/>
     </div>
  </xsl:for-each>
  <xsl:call-template name="footer"/>
  </body>
  </html>
</xsl:template>

<xsl:template match="comment">
</xsl:template>

<xsl:template match="book/preface">
  <div class="chapter">
    <xsl:apply-templates/>
  </div>
</xsl:template>

<xsl:template match="/preface">
  <html>
  <xsl:call-template name="page_head"/>
  <body>
  <div class="chapter">
    <xsl:apply-templates/>
  </div>
  <xsl:call-template name="footer"/>
  </body>
  </html>
</xsl:template>

<xsl:template match="preface/title">
  <p class="docname">
    <xsl:apply-templates/>
  </p>
</xsl:template>

<xsl:template match="chapter">
  <div class="chapter">
    <xsl:apply-templates/>
  </div>
</xsl:template>

<xsl:template match="/chapter">
  <html>
  <xsl:call-template name="page_head"/>
  <body>
  <div class="chapter">
    <xsl:apply-templates select="title"/>
    <xsl:apply-templates select="preface"/>
    <xsl:call-template name="chapter_content"/>
    <xsl:for-each select="section">
      <xsl:apply-templates/>
    </xsl:for-each>
  </div>
  <xsl:call-template name="footer"/>
  </body>
  </html>
</xsl:template>

<xsl:template match="chapter/title">
  <h1>
  <xsl:attribute name="id">
    <xsl:value-of select="../@id"/>
  </xsl:attribute>
  <xsl:text>Chapter </xsl:text><xsl:number count="chapter" format="1. "/>
  <xsl:apply-templates/>
  </h1>
</xsl:template>

<xsl:template match="chapter/preface">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="section">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="section/title">
  <h2>
  <xsl:attribute name="id">
    <xsl:value-of select="../@id"/>
  </xsl:attribute>
  <xsl:text>&#167; </xsl:text><xsl:number count="chapter|section" level="multiple" format="1.1. "/>
  <xsl:apply-templates/>
  </h2>
</xsl:template>


<xsl:template match="subsection">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="subsection/title">
  <h3>
  <xsl:attribute name="id">
    <xsl:value-of select="../@id"/>
  </xsl:attribute>
  <xsl:text>&#167; </xsl:text><xsl:number count="chapter|section|subsection" level="multiple" format="1.1.1. "/>
  <xsl:apply-templates/>
  </h3>
</xsl:template>

<!-- paragraph -->
<xsl:template match="p">
  <p>
  <xsl:if test="@id">
    <xsl:attribute name="id">
      <xsl:value-of select="@id"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </p>
</xsl:template>

<!-- note paragraph -->
<xsl:template match="note">
  <p><strong>Notes: </strong>
    <xsl:apply-templates/>
  </p>
</xsl:template>

<xsl:template match="li">
<li>
    <xsl:apply-templates/>
</li>
</xsl:template>

<xsl:template match="ol">
<ol>
  <xsl:if test="@start">
    <xsl:attribute name="start">
      <xsl:value-of select="@start"/>
    </xsl:attribute>
  </xsl:if>
<xsl:apply-templates/>
</ol>
</xsl:template>

<xsl:template match="ul">
<ul>
    <xsl:apply-templates/>
</ul>
</xsl:template>

<xsl:template match="pre">
  <pre>
  <xsl:if test="@class">
    <xsl:attribute name="class">
      <xsl:value-of select="@class"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </pre>
</xsl:template>

<!-- anchor -->
<xsl:template match="link">
  <a>
  <xsl:if test="@class">
    <xsl:attribute name="class">
      <xsl:value-of select="@class"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@href">
    <xsl:attribute name="href">
      <xsl:value-of select="@href"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </a>
</xsl:template>

<xsl:template match="label">
  <a>
  <xsl:if test="@class">
    <xsl:attribute name="class">
      <xsl:value-of select="@class"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@name">
    <xsl:attribute name="name">
      <xsl:value-of select="@name"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </a>
</xsl:template>

<xsl:template match="table">
  <table>
  <xsl:if test="@class">
    <xsl:attribute name="class">
      <xsl:value-of select="@class"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@width">
    <xsl:attribute name="width">
      <xsl:value-of select="@width"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </table>
</xsl:template>

<xsl:template match="table/caption">
  <caption>
  <!-- tables numeration across chapter like 'Table XX.YY: caption' -->
  <xsl:text>Table </xsl:text>
  <xsl:number count="chapter" format="1."/><xsl:number count="table" from="chapter" level="any" format="1: "/>
  <xsl:apply-templates/>
  </caption>
</xsl:template>

<xsl:template match="thead"> <thead> <xsl:apply-templates/> </thead></xsl:template>
<xsl:template match="tbody"> <tbody> <xsl:apply-templates/> </tbody></xsl:template>

<xsl:template match="th">
  <th>
  <xsl:if test="@class">
    <xsl:attribute name="class">
      <xsl:value-of select="@class"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@colspan">
    <xsl:attribute name="colspan">
      <xsl:value-of select="@colspan"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@rowspan">
    <xsl:attribute name="rowspan">
      <xsl:value-of select="@rowspan"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@width">
    <xsl:attribute name="style">
      <xsl:text>width:</xsl:text><xsl:value-of select="@width"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </th>
</xsl:template>


<xsl:template match="td">
  <td>
  <xsl:if test="@class">
    <xsl:attribute name="class">
      <xsl:value-of select="@class"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@colspan">
    <xsl:attribute name="colspan">
      <xsl:value-of select="@colspan"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@rowspan">
    <xsl:attribute name="rowspan">
      <xsl:value-of select="@rowspan"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@width">
    <xsl:attribute name="style">
      <xsl:text>width:</xsl:text><xsl:value-of select="@width"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </td>
</xsl:template>

<xsl:template match="tc">
  <td class="cnt">
  <xsl:if test="@colspan">
    <xsl:attribute name="colspan">
      <xsl:value-of select="@colspan"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@rowspan">
    <xsl:attribute name="rowspan">
      <xsl:value-of select="@rowspan"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@width">
    <xsl:attribute name="style">
      <xsl:text>width:</xsl:text><xsl:value-of select="@width"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </td>
</xsl:template>

<xsl:template match="cellc">
  <td style="text-align:center">
  <xsl:if test="@colspan">
    <xsl:attribute name="colspan">
      <xsl:value-of select="@colspan"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@rowspan">
    <xsl:attribute name="rowspan">
      <xsl:value-of select="@rowspan"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@width">
    <xsl:attribute name="style">
      <xsl:text>width:</xsl:text><xsl:value-of select="@width"/>
    </xsl:attribute>
  </xsl:if>
    <xsl:apply-templates/>
  </td>
</xsl:template>

<xsl:template match="tr|slot|reg_row">
  <tr>
    <xsl:apply-templates/>
  </tr>
</xsl:template>

<xsl:template match="code">
  <pre class="code">
    <xsl:apply-templates/>
  </pre>
</xsl:template>

<xsl:template match="strong">
  <strong>
    <xsl:apply-templates/>
  </strong>
</xsl:template>

<xsl:template match="bold">
  <b>
    <xsl:apply-templates/>
  </b>
</xsl:template>

<xsl:template match="var">
  <var>
    <xsl:apply-templates/>
  </var>
</xsl:template>

<xsl:template match="em"><em><xsl:apply-templates/></em></xsl:template>

<xsl:template match="sup">
  <sup>
    <xsl:apply-templates/>
  </sup>
</xsl:template>

<xsl:template match="sub">
  <sub>
    <xsl:apply-templates/>
  </sub>
</xsl:template>

<xsl:template match="br">
  <br/>
</xsl:template>

<xsl:template match="slot_table">
  <table class="ifmt">
    <xsl:apply-templates/>
  </table>
</xsl:template>

<xsl:template match="slot_table/caption">
  <caption>
    <xsl:apply-templates/>
  </caption>
</xsl:template>

<xsl:template match="reg_table">
  <table  class="v64">
    <xsl:apply-templates/>
  </table>
</xsl:template>

<xsl:template match="reg_table/caption">
  <caption>
    <xsl:apply-templates/>
  </caption>
</xsl:template>


<!-- ISA fields -->

<xsl:template match="opc">          <td class="opc" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="gpr">          <td class="gpr" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="spr">          <td class="spr" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="fpr">          <td class="fpr" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="vbitpos">      <td class="tdb" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="regzero">      <td class="res" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="rimm">         <td class="imm" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="cbimm">        <td class="imm" colspan="14">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="idisp">        <td class="imm" colspan="9">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="nmask">        <td class="imm" colspan="5">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="loop_step">    <td class="imm" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="prepare">      <td class="bpd" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="fused_opx">    <td class="opx" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="misc_opx">     <td class="opx" colspan="9">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="misc_opx_res"> <td class="res" colspan="5">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="raopx">        <td class="opx" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="nullify_opx">  <td class="opx" colspan="4">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="stride">       <td class="imm" colspan="14">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="binimm">       <td class="imm" colspan="21">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="binimmu">      <td class="imm" colspan="21">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="branch">       <td class="imm" colspan="21">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="cibranch">     <td class="imm" colspan="14">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="jump|call">    <td class="imm" colspan="28">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="trap">         <td class="imm" colspan="21">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="probe">        <td class="imm" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="fpcmode">      <td class="mod" colspan="5">   <xsl:apply-templates/> </td> </xsl:template>

<xsl:template match="scale">        <td class="res" colspan="2">0</td>  <td class="mod" colspan="3"> <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="roundmode">    <td class="res" colspan="2">0</td>  <td class="mod" colspan="3"> <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="memorder">     <td class="res" colspan="3">0</td>  <td class="mod" colspan="2"> <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="bitpos">       <td class="res">0</td>              <td class="tdb" colspan="6"> <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="zbitpos">      <td class="res">0</td>              <td class="tdb" colspan="6"> <xsl:apply-templates/> </td> </xsl:template>


<!-- cache hint (non-temporality) -->
<xsl:template match="cacheopx">
  <td class="mod">s</td>
  <td class="mod" colspan="2">nt</td>
  <td class="opx" colspan="4">
    <xsl:apply-templates/>
  </td>
</xsl:template>

<xsl:template match="noargs">
  <td class="res" colspan="21">0</td>
  <td class="res" colspan="5">0</td>
  <td class="opx" colspan="9"> <xsl:apply-templates/> </td>
</xsl:template>

<!-- variable sized reserved field -->
<xsl:template match="reserved">
  <td class="res">
  <xsl:if test="@size">
    <xsl:attribute name="colspan">
      <xsl:value-of select="@size"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@rowspan">
    <xsl:attribute name="rowspan">
      <xsl:value-of select="@rowspan"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </td>
</xsl:template>

<!-- variable sized ignored field -->
<xsl:template match="ignored">
  <td class="tdz">
  <xsl:if test="@size">
    <xsl:attribute name="colspan">
      <xsl:value-of select="@size"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </td>
</xsl:template>

<!-- variable sized immediate field -->
<xsl:template match="imm">
  <td class="imm">
  <xsl:if test="@size">
    <xsl:attribute name="colspan">
      <xsl:value-of select="@size"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </td>
</xsl:template>

<xsl:template match="bitfield">
  <td class="mod">
  <xsl:if test="@size">
    <xsl:attribute name="colspan">
      <xsl:value-of select="@size"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </td>
</xsl:template>

<xsl:template match="error">
  &#171;<b>
    <xsl:apply-templates/>
  </b>&#187;
</xsl:template>

<!-- instruction slot -->
<xsl:template match="slotbits">
<tr>
    <td class="bit">41</td>
    <td class="bit">40</td>
    <td class="bit">39</td>
    <td class="bit">38</td>
    <td class="bit">37</td>
    <td class="bit">36</td>
    <td class="bit">35</td>
    <td class="bit">34</td>
    <td class="bit">33</td>
    <td class="bit">32</td>
    <td class="bit">31</td>
    <td class="bit">30</td>
    <td class="bit">29</td>
    <td class="bit">28</td>
    <td class="bit">27</td>
    <td class="bit">26</td>
    <td class="bit">25</td>
    <td class="bit">24</td>
    <td class="bit">23</td>
    <td class="bit">22</td>
    <td class="bit">21</td>
    <td class="bit">20</td>
    <td class="bit">19</td>
    <td class="bit">18</td>
    <td class="bit">17</td>
    <td class="bit">16</td>
    <td class="bit">15</td>
    <td class="bit">14</td>
    <td class="bit">13</td>
    <td class="bit">12</td>
    <td class="bit">11</td>
    <td class="bit">10</td>
    <td class="bit">9</td>
    <td class="bit">8</td>
    <td class="bit">7</td>
    <td class="bit">6</td>
    <td class="bit">5</td>
    <td class="bit">4</td>
    <td class="bit">3</td>
    <td class="bit">2</td>
    <td class="bit">1</td>
    <td class="bit">0</td>
</tr>
<xsl:apply-templates/>
</xsl:template>

<!-- extended instruction slot -->
<xsl:template match="extslotbits">
<tr>
    <td class="bit">83</td>
    <td class="bit">82</td>
    <td class="bit">81</td>
    <td class="bit">80</td>
    <td class="bit">79</td>
    <td class="bit">78</td>
    <td class="bit">77</td>
    <td class="bit">76</td>
    <td class="bit">75</td>
    <td class="bit">74</td>
    <td class="bit">73</td>
    <td class="bit">72</td>
    <td class="bit">71</td>
    <td class="bit">70</td>
    <td class="bit">69</td>
    <td class="bit">68</td>
    <td class="bit">67</td>
    <td class="bit">66</td>
    <td class="bit">65</td>
    <td class="bit">64</td>
    <td class="bit">63</td>
    <td class="bit">62</td>
    <td class="bit">61</td>
    <td class="bit">60</td>
    <td class="bit">59</td>
    <td class="bit">58</td>
    <td class="bit">57</td>
    <td class="bit">56</td>
    <td class="bit">55</td>
    <td class="bit">54</td>
    <td class="bit">53</td>
    <td class="bit">52</td>
    <td class="bit">51</td>
    <td class="bit">50</td>
    <td class="bit">49</td>
    <td class="bit">48</td>
    <td class="bit">47</td>
    <td class="bit">46</td>
    <td class="bit">45</td>
    <td class="bit">44</td>
    <td class="bit">43</td>
    <td class="bit">42</td>
</tr>
<xsl:apply-templates/>
</xsl:template>

<!-- 64-bit template -->
<xsl:template match="bits64">
<tr>
    <td class="bit">6<br/>3</td>
    <td class="bit">6<br/>2</td>
    <td class="bit">6<br/>1</td>
    <td class="bit">6<br/>0</td>
    <td class="bit">5<br/>9</td>
    <td class="bit">5<br/>8</td>
    <td class="bit">5<br/>7</td>
    <td class="bit">5<br/>6</td>
    <td class="bit">5<br/>5</td>
    <td class="bit">5<br/>4</td>
    <td class="bit">5<br/>3</td>
    <td class="bit">5<br/>2</td>
    <td class="bit">5<br/>1</td>
    <td class="bit">5<br/>0</td>
    <td class="bit">4<br/>9</td>
    <td class="bit">4<br/>8</td>
    <td class="bit">4<br/>7</td>
    <td class="bit">4<br/>6</td>
    <td class="bit">4<br/>5</td>
    <td class="bit">4<br/>4</td>
    <td class="bit">4<br/>3</td>
    <td class="bit">4<br/>2</td>
    <td class="bit">4<br/>1</td>
    <td class="bit">4<br/>0</td>
    <td class="bit">3<br/>9</td>
    <td class="bit">3<br/>8</td>
    <td class="bit">3<br/>7</td>
    <td class="bit">3<br/>6</td>
    <td class="bit">3<br/>5</td>
    <td class="bit">3<br/>4</td>
    <td class="bit">3<br/>3</td>
    <td class="bit">3<br/>2</td>
    <td class="bit">3<br/>1</td>
    <td class="bit">3<br/>0</td>
    <td class="bit">2<br/>9</td>
    <td class="bit">2<br/>8</td>
    <td class="bit">2<br/>7</td>
    <td class="bit">2<br/>6</td>
    <td class="bit">2<br/>5</td>
    <td class="bit">2<br/>4</td>
    <td class="bit">2<br/>3</td>
    <td class="bit">2<br/>2</td>
    <td class="bit">2<br/>1</td>
    <td class="bit">2<br/>0</td>
    <td class="bit">1<br/>9</td>
    <td class="bit">1<br/>8</td>
    <td class="bit">1<br/>7</td>
    <td class="bit">1<br/>6</td>
    <td class="bit">1<br/>5</td>
    <td class="bit">1<br/>4</td>
    <td class="bit">1<br/>3</td>
    <td class="bit">1<br/>2</td>
    <td class="bit">1<br/>1</td>
    <td class="bit">1<br/>0</td>
    <td class="bit"> <br/>9</td>
    <td class="bit"> <br/>8</td>
    <td class="bit"> <br/>7</td>
    <td class="bit"> <br/>6</td>
    <td class="bit"> <br/>5</td>
    <td class="bit"> <br/>4</td>
    <td class="bit"> <br/>3</td>
    <td class="bit"> <br/>2</td>
    <td class="bit"> <br/>1</td>
    <td class="bit"> <br/>0</td>
</tr>
<xsl:apply-templates/>
</xsl:template>

<!-- 32-bit template (low part of 64-bit) -->
<xsl:template match="bits32">
<tr>
    <td class="bit">31</td>
    <td class="bit">30</td>
    <td class="bit">29</td>
    <td class="bit">28</td>
    <td class="bit">27</td>
    <td class="bit">26</td>
    <td class="bit">25</td>
    <td class="bit">24</td>
    <td class="bit">23</td>
    <td class="bit">22</td>
    <td class="bit">21</td>
    <td class="bit">20</td>
    <td class="bit">19</td>
    <td class="bit">18</td>
    <td class="bit">17</td>
    <td class="bit">16</td>
    <td class="bit">15</td>
    <td class="bit">14</td>
    <td class="bit">13</td>
    <td class="bit">12</td>
    <td class="bit">11</td>
    <td class="bit">10</td>
    <td class="bit">9</td>
    <td class="bit">8</td>
    <td class="bit">7</td>
    <td class="bit">6</td>
    <td class="bit">5</td>
    <td class="bit">4</td>
    <td class="bit">3</td>
    <td class="bit">2</td>
    <td class="bit">1</td>
    <td class="bit">0</td>
</tr>
<xsl:apply-templates/>
</xsl:template>

<!-- high part of 64-bit -->
<xsl:template match="hibits">
<tr>
    <td class="bit">63</td>
    <td class="bit">62</td>
    <td class="bit">61</td>
    <td class="bit">60</td>
    <td class="bit">59</td>
    <td class="bit">58</td>
    <td class="bit">57</td>
    <td class="bit">56</td>
    <td class="bit">55</td>
    <td class="bit">54</td>
    <td class="bit">53</td>
    <td class="bit">52</td>
    <td class="bit">51</td>
    <td class="bit">50</td>
    <td class="bit">49</td>
    <td class="bit">48</td>
    <td class="bit">47</td>
    <td class="bit">46</td>
    <td class="bit">45</td>
    <td class="bit">44</td>
    <td class="bit">43</td>
    <td class="bit">42</td>
    <td class="bit">41</td>
    <td class="bit">40</td>
    <td class="bit">39</td>
    <td class="bit">38</td>
    <td class="bit">37</td>
    <td class="bit">36</td>
    <td class="bit">35</td>
    <td class="bit">34</td>
    <td class="bit">33</td>
    <td class="bit">32</td>
</tr>
<xsl:apply-templates/>
</xsl:template>

<!-- quoted text -->
<xsl:template match="quote">&#171;<xsl:apply-templates/>&#187;</xsl:template>

<!-- Definitions -->
<xsl:template match="def">
  <dfn>
    <xsl:apply-templates/>
  </dfn>
</xsl:template>

<!-- Relocation types -->
<xsl:template match="relocation">
  <span class="relocation">
    <xsl:apply-templates/>
  </span>
</xsl:template>

<xsl:template match="formula">
  <span class="formula">
    <xsl:apply-templates/>
  </span>
</xsl:template>

<xsl:template match="type">
  <span class="type">
    <xsl:apply-templates/>
  </span>
</xsl:template>

<xsl:template match="function">
  <span class="function">
    <xsl:apply-templates/>
  </span>
</xsl:template>

<!-- Machine Register Mnemonic -->
<xsl:template match="reg">
  <span class="reg">
    <xsl:apply-templates/>
  </span>
</xsl:template>

<!-- Machine Register Full Name -->
<xsl:template match="regname">
  <span class="regname">
    <xsl:apply-templates/>
  </span>
</xsl:template>

<!-- Machine Register Fields -->
<xsl:template match="regfield">
  <span class="regfield">
    <xsl:apply-templates/>
  </span>
</xsl:template>

<!-- Machine Instruction Mnemonics -->
<xsl:template match="mim">
  <span class="mim">
    <xsl:apply-templates/>
  </span>
</xsl:template>

<!-- Machine Instruction Full Name -->
<xsl:template match="miname">
  <span class="miname">
    <xsl:apply-templates/>
  </span>
</xsl:template>

<!-- Machine Instruction/Function Parameter -->
<xsl:template match="param">
  <span class="param">
    <xsl:apply-templates/>
  </span>
</xsl:template>


<xsl:template match="shy">&#173;</xsl:template>    <!-- soft word-break -->
<xsl:template match="nbsp">&#160;</xsl:template>
<xsl:template match="sect">&#167;</xsl:template>
<xsl:template match="copy">&#169;</xsl:template>
<xsl:template match="laquo">&#171;</xsl:template>
<xsl:template match="not">&#172;</xsl:template>
<xsl:template match="plusmn">&#177;</xsl:template>
<xsl:template match="raquo">&#187;</xsl:template>
<xsl:template match="times">&#215;</xsl:template>
<xsl:template match="ndash">&#8211;</xsl:template>
<xsl:template match="mdash">&#8212;</xsl:template>
<xsl:template match="lsquo">&#8216;</xsl:template>
<xsl:template match="rsquo">&#8217;</xsl:template>
<xsl:template match="sbquo">&#8218;</xsl:template>
<xsl:template match="ldquo">&#8220;</xsl:template>
<xsl:template match="rdquo">&#8221;</xsl:template>
<xsl:template match="bdquo">&#8222;</xsl:template>
<xsl:template match="lsaquo">&#8249;</xsl:template> 
<xsl:template match="rsaquo">&#8250;</xsl:template>
<xsl:template match="minus">&#8722;</xsl:template>
<xsl:template match="hellip">&#8230;</xsl:template>
<xsl:template match="larr">&#8592;</xsl:template>  <!-- left arrow -->
<xsl:template match="uarr">&#8593;</xsl:template>  <!-- up arrow -->
<xsl:template match="rarr">&#8594;</xsl:template>  <!-- right arrow -->
<xsl:template match="darr">&#8595;</xsl:template>  <!-- down arrow -->
<xsl:template match="harr">&#8596;</xsl:template>  <!-- left-right arrow -->
<xsl:template match="ne">&#8800;</xsl:template>    <!-- not equal -->
<xsl:template match="equiv">&#8801;</xsl:template>  <!-- equivalence -->
<xsl:template match="le">&#8804;</xsl:template>    <!-- less than or equal -->
<xsl:template match="ge">&#8805;</xsl:template>    <!-- greater than or equal -->
<xsl:template match="oplus">&#8853;</xsl:template>
<xsl:template match="lshift">&lt;&lt;</xsl:template>
<xsl:template match="rshift">&gt;&gt;</xsl:template>

<!-- XML DEFAULT RULES -->
<xsl:template match="*|/">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="text()">
  <xsl:value-of select="."/>
</xsl:template>

<xsl:template match="@*">
  <xsl:value-of select="."/>
</xsl:template>

</xsl:stylesheet>
