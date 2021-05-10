<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
  xmlns="http://www.w3.org/1999/xhtml"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:msxsl="urn:schemas-microsoft-com:xslt"
  xmlns:exslt="http://exslt.org/common"
  xmlns:str="http://exslt.org/strings"
  xmlns:doc="http://www.example.com/postrisc"
  extension-element-prefixes="msxsl exslt str"
  exclude-result-prefixes="doc"
>

<!--
  xmlns:date="http://exslt.org/dates-and-times"
  xmlns:xhtml="http://www.w3.org/1999/xhtml"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:fo="http://www.w3.org/1999/Format"
  xmlns:xi="http://www.w3.org/2001/XInclude"
  method="html"
  version="1.1"
  doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"
  doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"
-->

<!-- possible `stringparam` params in CMakeLists.txt -->
<xsl:param name="PACKAGE_VERSION"/>
<xsl:param name="PACKAGE_DATE"/>
<xsl:param name="PACKAGE_DATE_YEAR"/>

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
<xsl:preserve-space elements="doc:p"/>
<xsl:preserve-space elements="doc:td"/>
<xsl:preserve-space elements="doc:li"/>
<xsl:preserve-space elements="doc:note"/>


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
  <xsl:value-of select="document(@href)/doc:chapter/doc:title"/>
  </a></p>
</xsl:template>

<xsl:template match="prebook">
  <!-- preload all includes into root non-xhtml 'book' tag without default namespace -->
  <xsl:variable name="first">
    <xsl:element name="book" namespace="">  <!-- namespace="doc" http://www.example.com/postrisc -->
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

<xsl:template match="prebook/doc:title">
  <title>
    <xsl:apply-templates/>
  </title>
</xsl:template>

<xsl:template match="doc:precontent">
<content/>
</xsl:template>

<!--
second stage tags
-->

<xsl:template match="doc:archname">POSTRISC</xsl:template>

<xsl:template name="footer">
<div class="chapter">
<br/><br/><hr/>
<p class="copyright">POSTRISC virtual processor.</p>
<p class="copyright">Instruction Set Architecture (ISA) reference manual.</p>
<p class="copyright">Copyright &#169; 2003-<xsl:choose>
<xsl:when test="not($PACKAGE_DATE_YEAR)">2023</xsl:when>
<xsl:otherwise><xsl:value-of select="$PACKAGE_DATE_YEAR"/></xsl:otherwise>
</xsl:choose> by Dmitry Buvaylo.</p>
</div>
</xsl:template>

<xsl:template name="chapter_content">
  <xsl:if test="not(parent::book)">
    <h2>Content</h2>
  </xsl:if>
  <xsl:for-each select="doc:section">
    <p class="toc2"><xsl:text>&#167; </xsl:text><xsl:number count="doc:chapter|doc:section" level="multiple" format="1.1. "/>
    <a>
    <xsl:attribute name="href">
      <xsl:value-of select="@href"/>#<xsl:value-of select="@id"/>
    </xsl:attribute>
    <xsl:for-each select="doc:title">
      <xsl:apply-templates/>
    </xsl:for-each>
    </a></p>
    <xsl:for-each select="doc:subsection">
      <p class="toc3"><xsl:text>&#167; </xsl:text><xsl:number count="doc:chapter|doc:section|doc:subsection" level="multiple" format="1.1.1. "/>
      <a>
      <xsl:attribute name="href">
        <xsl:value-of select="@href"/>#<xsl:value-of select="@id"/>
      </xsl:attribute>
      <xsl:for-each select="doc:title">
        <xsl:apply-templates/>
      </xsl:for-each>
      </a></p>
    </xsl:for-each>
  </xsl:for-each>
</xsl:template>

<xsl:template name="book_content">
  <div class="chapter">
  <h1>Content</h1>
  <xsl:for-each select="doc:chapter">
    <p class="toc1"><xsl:text>Chapter </xsl:text><xsl:number count="doc:chapter" format="1. "/>
    <a>
    <xsl:attribute name="href">
      <xsl:value-of select="@href"/>#<xsl:value-of select="@id"/>
    </xsl:attribute>
    <xsl:for-each select="doc:title">
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
    <xsl:for-each select="./doc:preface/doc:title">
      <xsl:apply-templates/>
    </xsl:for-each>
  </title>
  <meta http-equiv="content-type" content="text/html; charset=utf-8"/>
  <meta http-equiv="content-language" content="en"/>
  <meta http-equiv="Content-Style-Type" content="text/css"/>
  <meta name="description" content="Postrisc instruction set arhitecture"/>
  <meta name="viewport" content="width=device-width"/>
  <meta name="language" content="en"/>
  <link href="styles.css" rel="stylesheet" type="text/css"/>
  </head>
</xsl:template>

<xsl:template match="book">
  <html lang="en">
  <xsl:call-template name="page_head"/>
  <body>
  <xsl:apply-templates select="./doc:preface"/>
  <xsl:call-template name="book_content"/>
  <xsl:for-each select="./doc:chapter">
     <div class="chapter">
       <xsl:apply-templates/>
     </div>
  </xsl:for-each>
  <xsl:call-template name="footer"/>
  </body>
  </html>
</xsl:template>

<xsl:template match="doc:comment">
</xsl:template>

<xsl:template match="book/doc:preface">
  <div class="chapter">
    <xsl:apply-templates/>
  </div>
</xsl:template>

<xsl:template match="/doc:preface">
  <html lang="en">
  <xsl:call-template name="page_head"/>
  <body>
  <div class="chapter">
    <xsl:apply-templates/>
  </div>
  <xsl:call-template name="footer"/>
  </body>
  </html>
</xsl:template>

<xsl:template match="doc:preface/doc:title">
  <p class="docname">
    <xsl:apply-templates/>
  </p>
</xsl:template>

<xsl:template match="doc:chapter">
  <div class="chapter">
    <xsl:apply-templates/>
  </div>
</xsl:template>

<xsl:template match="/doc:chapter">
  <html lang="en">
  <xsl:call-template name="page_head"/>
  <body>
  <div class="chapter">
    <xsl:apply-templates select="doc:title"/>
    <xsl:apply-templates select="doc:preface"/>
    <xsl:call-template name="chapter_content"/>
    <xsl:for-each select="doc:section">
      <xsl:apply-templates/>
    </xsl:for-each>
  </div>
  <xsl:call-template name="footer"/>
  </body>
  </html>
</xsl:template>

<xsl:template match="doc:chapter/doc:title">
  <h1>
  <xsl:attribute name="id">
    <xsl:value-of select="../@id"/>
  </xsl:attribute>
  <xsl:text>Chapter </xsl:text><xsl:number count="doc:chapter" format="1. "/>
  <xsl:apply-templates/>
  </h1>
</xsl:template>

<xsl:template match="doc:chapter/doc:preface">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="doc:section">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="doc:section/doc:title">
  <h2>
  <xsl:attribute name="id">
    <xsl:value-of select="../@id"/>
  </xsl:attribute>
  <xsl:text>&#167; </xsl:text>
  <xsl:number count="doc:chapter|doc:section" level="multiple" format="1.1. "/>
  <xsl:apply-templates/>
  </h2>
</xsl:template>


<xsl:template match="doc:subsection">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="doc:subsection/doc:title">
  <h3>
    <xsl:attribute name="id">
      <xsl:value-of select="../@id"/>
    </xsl:attribute>
    <xsl:text>&#167; </xsl:text>
    <xsl:number count="doc:chapter|doc:section|doc:subsection" level="multiple" format="1.1.1. "/>
    <xsl:apply-templates/>
  </h3>
</xsl:template>

<!-- paragraph -->
<xsl:template match="doc:p">
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
<xsl:template match="doc:note">
  <p><strong>Notes: </strong>
    <xsl:apply-templates/>
  </p>
</xsl:template>

<xsl:template match="doc:li">
<li>
    <xsl:apply-templates/>
</li>
</xsl:template>

<xsl:template match="doc:ol">
<ol>
  <xsl:if test="@start">
    <xsl:attribute name="start">
      <xsl:value-of select="@start"/>
    </xsl:attribute>
  </xsl:if>
<xsl:apply-templates/>
</ol>
</xsl:template>

<xsl:template match="doc:ul">
  <ul>
    <xsl:apply-templates/>
  </ul>
</xsl:template>

<xsl:template match="doc:pre">
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
<xsl:template match="doc:link">
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

<xsl:template match="doc:label">
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

<xsl:template match="doc:table">
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

<xsl:template match="doc:table/doc:caption">
  <caption>
    <!-- tables numeration across chapter like 'Table XX.YY: caption' -->
    <xsl:text>Table </xsl:text>
    <xsl:number count="doc:chapter" format="1."/>
    <xsl:number count="doc:table" from="doc:chapter" level="any" format="1: "/>
  <xsl:apply-templates/>
  </caption>
</xsl:template>

<xsl:template match="doc:thead">
  <thead>
    <xsl:apply-templates/>
  </thead>
</xsl:template>

<xsl:template match="doc:tbody">
  <tbody>
    <xsl:apply-templates/>
  </tbody>
</xsl:template>

<xsl:template match="doc:th">
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


<xsl:template match="doc:td">
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

<xsl:template match="doc:tc">
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

<xsl:template match="doc:cellc">
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

<xsl:template match="doc:tr|doc:slot|doc:reg_row">
  <tr>
    <xsl:apply-templates/>
  </tr>
</xsl:template>

<xsl:template match="doc:code">
  <pre class="code">
    <xsl:apply-templates/>
  </pre>
</xsl:template>

<xsl:template match="doc:strong">
  <strong>
    <xsl:apply-templates/>
  </strong>
</xsl:template>

<xsl:template match="doc:bold">
  <b>
    <xsl:apply-templates/>
  </b>
</xsl:template>

<xsl:template match="doc:var">
  <var>
    <xsl:apply-templates/>
  </var>
</xsl:template>

<xsl:template match="doc:em">
  <em>
    <xsl:apply-templates/>
  </em>
</xsl:template>

<xsl:template match="doc:sup">
  <sup>
    <xsl:apply-templates/>
  </sup>
</xsl:template>

<xsl:template match="doc:sub">
  <sub>
    <xsl:apply-templates/>
  </sub>
</xsl:template>

<xsl:template match="doc:br">
  <br/>
</xsl:template>

<xsl:template match="doc:slot_table">
  <table class="ifmt">
    <xsl:apply-templates/>
  </table>
</xsl:template>

<xsl:template match="doc:slot_table/doc:caption">
  <caption>
    <xsl:apply-templates/>
  </caption>
</xsl:template>

<xsl:template match="doc:reg_table">
  <table class="v64">
    <xsl:apply-templates/>
  </table>
</xsl:template>

<xsl:template match="doc:reg_table/doc:caption">
  <caption>
    <xsl:apply-templates/>
  </caption>
</xsl:template>


<!-- ISA fields -->

<xsl:template match="doc:opc">              <td class="opc" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:gpr">              <td class="gpr" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:spr">              <td class="spr" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:fpr">              <td class="fpr" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:bitpos">           <td class="tdb" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:regzero">          <td class="res" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:cbimm">            <td class="imm" colspan="11">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:idisp">            <td class="imm" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:nmask">            <td class="imm" colspan="5">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:prepare">          <td class="bpd" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:fused_opx">        <td class="opx" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:misc_opx">         <td class="opx" colspan="11">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:misc_opx_res">     <td class="res" colspan="3">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:raopx">            <td class="opx" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:loop_opx">         <td class="opx" colspan="4">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:stride">           <td class="imm" colspan="10">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:binimm">           <td class="imm" colspan="21">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:mid_binimm">       <td class="imm" colspan="14">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:branch">           <td class="imm" colspan="17">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:branch_opx">       <td class="opx" colspan="4">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:branch_opx_res">   <td class="res" colspan="4">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:jump|doc:call">    <td class="imm" colspan="28">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:trap">             <td class="imm" colspan="22">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:probe">            <td class="imm" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:nullify_opx">      <td class="opx" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:framesize">        <td class="imm" colspan="7">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:scale">            <td class="mod" colspan="3">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:indexed_opx">      <td class="opx" colspan="4">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:res21">            <td class="res" colspan="21">  <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:loop_stride">      <td class="imm" colspan="6">   <xsl:apply-templates/> </td> </xsl:template>
<xsl:template match="doc:loop_disp">        <td class="imm" colspan="11">  <xsl:apply-templates/> </td> </xsl:template>


<!-- cache hint (non-temporality) -->
<xsl:template match="doc:cacheopx">
  <td class="mod">s</td>
  <td class="mod" colspan="2">nt</td>
  <td class="opx" colspan="4">
    <xsl:apply-templates/>
  </td>
</xsl:template>

<xsl:template match="doc:noargs">
  <td class="res" colspan="24">0</td>
  <td class="opx" colspan="11"> <xsl:apply-templates/> </td>
</xsl:template>

<!-- variable sized reserved field -->
<xsl:template match="doc:reserved">
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
<xsl:template match="doc:ignored">
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
<xsl:template match="doc:imm">
  <td class="imm">
  <xsl:if test="@size">
    <xsl:attribute name="colspan">
      <xsl:value-of select="@size"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </td>
</xsl:template>

<xsl:template match="doc:bitfield">
  <td class="mod">
  <xsl:if test="@size">
    <xsl:attribute name="colspan">
      <xsl:value-of select="@size"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </td>
</xsl:template>

<xsl:template match="doc:bit">
  <td class="imm">
  <xsl:if test="@size">
    <xsl:attribute name="colspan">
      <xsl:value-of select="@size"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:apply-templates/>
  </td>
</xsl:template>

<!-- instruction slot -->
<xsl:template match="doc:slotbits">
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
<xsl:template match="doc:extslotbits">
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
<xsl:template match="doc:bits64">
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
<xsl:template match="doc:bits32">
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
<xsl:template match="doc:hibits">
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

<!-- 16-bit template -->
<xsl:template match="doc:bits16">
<tr>
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

<!-- quoted text -->
<xsl:template match="doc:quote">&#171;<xsl:apply-templates/>&#187;</xsl:template>
<!-- quoted error text -->
<xsl:template match="doc:error">
  &#171;<b>
    <xsl:apply-templates/>
</b>&#187;</xsl:template>

<!-- Definitions -->
<xsl:template match="doc:def">
  <dfn>
    <xsl:apply-templates/>
  </dfn>
</xsl:template>

<xsl:template match="doc:relocation"> <span class="relocation"> <xsl:apply-templates/> </span> </xsl:template>
<xsl:template match="doc:formula">    <span class="formula">    <xsl:apply-templates/> </span> </xsl:template>
<xsl:template match="doc:type">       <span class="type">       <xsl:apply-templates/> </span> </xsl:template>
<xsl:template match="doc:function">   <span class="function">   <xsl:apply-templates/> </span> </xsl:template>
<xsl:template match="doc:reg">        <span class="reg">        <xsl:apply-templates/> </span> </xsl:template>
<xsl:template match="doc:regname">    <span class="regname">    <xsl:apply-templates/> </span> </xsl:template>
<xsl:template match="doc:regfield">   <span class="regfield">   <xsl:apply-templates/> </span> </xsl:template>
<xsl:template match="doc:mim">        <span class="mim">        <xsl:apply-templates/> </span> </xsl:template>
<xsl:template match="doc:miname">     <span class="miname">     <xsl:apply-templates/> </span> </xsl:template>
<xsl:template match="doc:param">      <span class="param">      <xsl:apply-templates/> </span> </xsl:template>


<xsl:template match="doc:shy">&#173;</xsl:template>    <!-- soft word-break -->
<xsl:template match="doc:nbsp">&#160;</xsl:template>
<xsl:template match="doc:sect">&#167;</xsl:template>
<xsl:template match="doc:copy">&#169;</xsl:template>
<xsl:template match="doc:laquo">&#171;</xsl:template>
<xsl:template match="doc:not">&#172;</xsl:template>
<xsl:template match="doc:plusmn">&#177;</xsl:template>
<xsl:template match="doc:raquo">&#187;</xsl:template>
<xsl:template match="doc:times">&#215;</xsl:template>
<xsl:template match="doc:ndash">&#8211;</xsl:template>
<xsl:template match="doc:mdash">&#8212;</xsl:template>
<xsl:template match="doc:lsquo">&#8216;</xsl:template>
<xsl:template match="doc:rsquo">&#8217;</xsl:template>
<xsl:template match="doc:sbquo">&#8218;</xsl:template>
<xsl:template match="doc:ldquo">&#8220;</xsl:template>
<xsl:template match="doc:rdquo">&#8221;</xsl:template>
<xsl:template match="doc:bdquo">&#8222;</xsl:template>
<xsl:template match="doc:lsaquo">&#8249;</xsl:template> 
<xsl:template match="doc:rsaquo">&#8250;</xsl:template>
<xsl:template match="doc:minus">&#8722;</xsl:template>
<xsl:template match="doc:hellip">&#8230;</xsl:template>
<xsl:template match="doc:larr">&#8592;</xsl:template>  <!-- left arrow -->
<xsl:template match="doc:uarr">&#8593;</xsl:template>  <!-- up arrow -->
<xsl:template match="doc:rarr">&#8594;</xsl:template>  <!-- right arrow -->
<xsl:template match="doc:darr">&#8595;</xsl:template>  <!-- down arrow -->
<xsl:template match="doc:harr">&#8596;</xsl:template>  <!-- left-right arrow -->
<xsl:template match="doc:ne">&#8800;</xsl:template>    <!-- not equal -->
<xsl:template match="doc:equiv">&#8801;</xsl:template>  <!-- equivalence -->
<xsl:template match="doc:le">&#8804;</xsl:template>    <!-- less than or equal -->
<xsl:template match="doc:ge">&#8805;</xsl:template>    <!-- greater than or equal -->
<xsl:template match="doc:oplus">&#8853;</xsl:template>
<xsl:template match="doc:lt">&lt;</xsl:template>
<xsl:template match="doc:gt">&gt;</xsl:template>
<xsl:template match="doc:lshift">&lt;&lt;</xsl:template>
<xsl:template match="doc:rshift">&gt;&gt;</xsl:template>

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
